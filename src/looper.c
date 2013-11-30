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
    LOOPER_INPUT  = 0,
    LOOPER_OUTPUT = 1,
    LOOPER_RECORD = 2
} PortIndex;

typedef struct {
    float*       buffer;
    size_t       end;
    size_t       pos;
} Loop;

typedef struct {
    const float* input;
    float*       output;
    const float* record;
    Loop*  loop;
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
        looper->record = (const float*)data;
        break;
    }
}

static void
activate(LV2_Handle instance)
{
    Looper* looper = (Looper*)instance;
    looper->loop->pos = 0;
    looper->loop->end = 0;
}

/** Process a block of audio (audio thread, must be RT safe). */
static void
run(LV2_Handle instance, uint32_t n_samples)
{
    Looper* looper = (Looper*)instance;
    Loop*   loop   = looper->loop;

    const float* const input  = looper->input;
    float* const       output = looper->output;
    const float* const record = looper->record;

    if (*record != 0.0)
    {
        memcpy(&(loop->buffer[loop->pos]), input, n_samples * sizeof(float));
        loop->pos += n_samples;
        loop->end += n_samples;
        memset(output, 0, n_samples * sizeof(float));
    }
    else if ((loop->pos + n_samples) <= loop->end)
    {
        memcpy(output, &(loop->buffer[loop->pos]), n_samples * sizeof(float));
        loop->pos += n_samples;
    }
    else if (loop->end >= n_samples)
    {
        memcpy(output, loop->buffer, n_samples * sizeof(float));
        loop->pos = n_samples;
    }
    else
    {
        memset(output, 0, n_samples * sizeof(float));
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

