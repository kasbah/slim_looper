// This file is part of SLim Looper.
// 
// SLim Looper is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 3 as 
// published by the Free Software Foundation.
// 
// SLim Looper is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with SLim Looper. If not, see <http://www.gnu.org/licenses/>

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "lv2/lv2plug.in/ns/lv2core/lv2.h"

#define LOOPER_URI "https://github.com/kasbah/slim_looper.lv2"
#define LOOP_MAX_SAMPLES 19200000 

typedef enum {
    LOOPER_INPUT        = 0,
    LOOPER_OUTPUT       = 1,
    LOOPER_RECORD       = 2,
    LOOPER_PAUSE        = 3,
    LOOPER_RECORD_MODE  = 4
} PortIndex;

typedef struct {
    float*       buffer;
    size_t       end;
    size_t       pos;
} Loop;

typedef struct {
    uint8_t record;
    uint8_t pause;
} LooperControlState;

typedef enum {
    MODE_NEW     = 0,
    MODE_OVERDUB = 1,
    MODE_INSERT  = 2,
    MODE_REPLACE = 3
} LooperRecordMode;

typedef struct {
    LooperRecordMode record_mode;
} LooperSettings;

typedef enum {
    PLAYING   = 0,
    PAUSED    = 1,
    RECORDING = 2
} LooperState;

typedef struct {
    const float* input;
    float*       output;
    const float* record_input;
    const float* pause_input;
    const float* record_mode_input;
    LooperControlState* controls;
    Loop*  loop;
    LooperState state;
    LooperSettings* settings;
    uint8_t previous_state;
    uint8_t prev_record_input;
} Looper;

static LV2_Handle
instantiate(const LV2_Descriptor*     descriptor,
            double                    rate,
            const char*               bundle_path,
            const LV2_Feature* const* features)
{
    Looper* looper = (Looper*)malloc(sizeof(Looper));
    looper->loop = (Loop*)malloc(sizeof(Loop));
    looper->loop->buffer = calloc(LOOP_MAX_SAMPLES, sizeof(float));
    looper->controls = (LooperControlState*)malloc(sizeof(LooperControlState));
    looper->settings = (LooperSettings*)malloc(sizeof(LooperSettings));

    return (LV2_Handle)looper;
}

static void
connect_port(LV2_Handle instance,
             uint32_t   port,
             void*      data)
{
    Looper* looper = (Looper*)instance;

    switch ((PortIndex)port) {
    case LOOPER_INPUT:
        looper->input = (const float*)data;
        break;
    case LOOPER_OUTPUT:
        looper->output = (float*)data;
        break;
    case LOOPER_RECORD:
        looper->record_input = (const float*)data;
        break;
    case LOOPER_PAUSE:
        looper->pause_input = (const float*)data;
        break;
    case LOOPER_RECORD_MODE:
        looper->record_mode_input = (const float*)data;
        break;
    }
}

static void
activate(LV2_Handle instance)
{
    Looper* looper = (Looper*)instance;
    looper->loop->pos = 0;
    looper->loop->end = 0;
    looper->controls->record = 0;
    looper->controls->pause = 0;
    looper->state = PLAYING;
    looper->previous_state = PLAYING;
    looper->settings->record_mode = MODE_NEW;
}

/** Process a block of audio (audio thread, must be RT safe). */
static void
run(LV2_Handle instance, uint32_t n_samples)
{
    Looper*             looper   = (Looper*)instance;
    Loop*               loop     = looper->loop;
    LooperControlState* controls = looper->controls;
    LooperSettings*     settings = looper->settings;

    const float* const input  = looper->input;
    float* const       output = looper->output;

    controls->record = ((*(looper->record_input) != 0.0) ? 1 : 0);
    controls->pause  = ((*(looper->pause_input ) != 0.0) ? 1 : 0);

    settings->record_mode  = (LooperRecordMode)(*(looper->record_mode_input));
    
    if (controls->record)
    {
      looper->state = RECORDING;
      if (looper->previous_state != RECORDING && settings->record_mode == MODE_NEW)
      {
          loop->pos = 0;
          loop->end = 0;
      }
    }
    else if (controls->pause)
    {
      looper->state = PAUSED;
    }
    else
    {
      looper->state = PLAYING;
    }

    looper->previous_state = looper->state;

    switch(looper->state)
    {
        case RECORDING:
            {
                if (settings->record_mode == MODE_NEW)
                {
                    memcpy(&(loop->buffer[loop->pos]), input, n_samples * sizeof(float));
                    loop->pos += n_samples;
                    loop->end += n_samples;
                    memset(output, 0, n_samples * sizeof(float));
                }
                else if (settings->record_mode == MODE_OVERDUB)
                {
                    // position is before loop end
                    if ((loop->pos + n_samples) <= (loop->end)) 
                    {
                        for (int i = 0; i < n_samples; i++)
                        {
                            loop->buffer[loop->pos + i] += input[i]; //TODO: reduce gain to stop clipping 
                        }
                        loop->pos += n_samples;
                    }
                    //position is greater than loop length 
                    //looping around to start as long as we have a loop
                    else if (loop->end >= n_samples) 
                    {
                        for (int i = 0; i < n_samples; i++)
                        {
                            loop->buffer[i] += input[i]; //TODO: reduce gain to stop clipping 
                        }
                        memcpy(output, loop->buffer, n_samples * sizeof(float));
                        loop->pos = n_samples;
                    }
                    else //no loop, output silence
                    {
                        memset(output, 0, n_samples * sizeof(float));
                    }
                }
            }
            break;
        case PAUSED:
            {
                memset(output, 0, n_samples * sizeof(float));
            }
            break;
        case PLAYING:
        default:
            {
                // position is before loop end
                if ((loop->pos + n_samples) <= loop->end)
                {
                    memcpy(output, &(loop->buffer[loop->pos]), n_samples * sizeof(float));
                    loop->pos += n_samples;
                }
                //position is greater than loop length 
                //looping around to start as long as we have a loop
                else if (loop->end >= n_samples)
                {
                    memcpy(output, loop->buffer, n_samples * sizeof(float));
                    loop->pos = n_samples;
                }
                else //no loop, output silence
                {
                    memset(output, 0, n_samples * sizeof(float));
                }
            }
            break;
    }
}

static void
deactivate(LV2_Handle instance)
{
}

static void
cleanup(LV2_Handle instance)
{
    Looper* looper = (Looper*)instance;
    free(looper->loop->buffer);
    free(looper->loop);
    free(looper);
}

static const void*
extension_data(const char* uri)
{
    return NULL;
}

static const LV2_Descriptor descriptor = {
    LOOPER_URI,
    instantiate,
    connect_port,
    activate,
    run,
    deactivate,
    cleanup,
    extension_data
};

LV2_SYMBOL_EXPORT
const LV2_Descriptor*
lv2_descriptor(uint32_t index)
{
    switch (index) {
    case 0:
        return &descriptor;
    default:
        return NULL;
    }
}

