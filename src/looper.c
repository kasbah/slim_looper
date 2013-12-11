//
// copyright 2013-2014 Kaspar Emanuel
//
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
//
#include "looper.h"
#include <stdlib.h>
#include <string.h>

Looper* looper_new(uint32_t max_n_samples)
{
    Looper* looper = (Looper*)malloc(sizeof(Looper));
    looper->output = calloc(max_n_samples, sizeof(float));
    looper->loop   = (Loop*)malloc(sizeof(Loop));
    looper->loop->buffer = calloc(LOOP_MAX_SAMPLES, sizeof(float));
    looper->settings     = (LooperSettings*)malloc(sizeof(LooperSettings));
    return looper;
}

void looper_free(Looper* looper)
{
    free(looper->loop->buffer);
    free(looper->loop);
    free(looper);
}

void looper_reset(Looper* looper)
{
    looper->loop->pos = 0;
    looper->loop->end = 0;
    looper->settings->state = PAUSED;
    looper->previous_state = PAUSED;
    looper->settings->record_mode = MODE_NEW;
}

void looper_run(Looper* looper, uint32_t n_samples)
{
    switch(looper->settings->state)
    {
        case RECORDING:
            looper_record(looper, n_samples);
            break;
        case PLAYING:
            looper_play(looper, n_samples);
            break;
        case PAUSED:
        default:
            memset(looper->output, 0, n_samples * sizeof(float));
            break;
    }

    looper->previous_state = looper->settings->state;
}

// is position after processing nsamples before loop end?
static uint8_t loop_pos_before_end(Loop* loop, uint32_t n_samples)
{
    return ((loop->pos + n_samples) <= (loop->end));
}

// existence is defined as being at least one block long
static uint8_t loop_exists(Loop* loop, uint32_t n_samples)
{
    return (loop->end >= n_samples);
}


void
looper_record(Looper* looper, uint32_t n_samples)
{
    const float* const input  = looper->input;
    float* const       output = looper->output;

    Loop*               loop     = looper->loop;
    LooperSettings*     settings = looper->settings;

    switch(settings->record_mode) 
    {
        case MODE_NEW:
            //reset if we are not already running
            if (looper->previous_state != RECORDING)
            {
                loop->pos = 0;
                loop->end = 0;
            }
            memcpy(&(loop->buffer[loop->pos]), input, n_samples * sizeof(float));
            loop->pos += n_samples;
            loop->end += n_samples;
            memset(output, 0, n_samples * sizeof(float));
            break;

        case MODE_OVERDUB:
            if (loop_pos_before_end(loop, n_samples)) 
            {
                memcpy(output, &(loop->buffer[loop->pos]), n_samples * sizeof(float));
                for (int i = 0; i < n_samples; i++)
                {   //TODO: reduce gain to stop clipping 
                    loop->buffer[loop->pos + i] += input[i];
                }
                loop->pos += n_samples;
            }
            //position is greater than loop length 
            //looping around to start as long as we have a loop
            else if (loop_exists(loop, n_samples)) 
            {
                memcpy(output, loop->buffer, n_samples * sizeof(float));
                for (int i = 0; i < n_samples; i++)
                {//TODO: reduce gain to stop clipping 
                    loop->buffer[i] += input[i];
                }
                loop->pos = n_samples;
            }
            else //no loop, output silence
            {
                memset(output, 0, n_samples * sizeof(float));
            }
            break;

        case MODE_INSERT:
            if (loop_exists(loop, n_samples))
            {
                memmove(&(loop->buffer[loop->pos + n_samples]), &(loop->buffer[loop->pos]), (loop->end - loop->pos) * sizeof(float));
                memcpy(&(loop->buffer[loop->pos]), input, n_samples * sizeof(float));
                loop->pos += n_samples;
                loop->end += n_samples;
            }
            memset(output, 0, n_samples * sizeof(float));
            break;

        case MODE_REPLACE:
            if (loop_pos_before_end(loop, n_samples)) 
            {
                memcpy(&(loop->buffer[loop->pos]), input, n_samples * sizeof(float));
                loop->pos += n_samples;
            }
            //position is greater than loop length 
            //looping around to start as long as we have a loop
            else if (loop_exists(loop, n_samples))
            {
                memcpy(loop->buffer, input, n_samples * sizeof(float));
                loop->pos = n_samples;
            }
            memset(output, 0, n_samples * sizeof(float));
            break;

        default:
            break;
    }
}

void
looper_play(Looper* looper, uint32_t n_samples)
{
    const float* const input  = looper->input;
    float* const       output = looper->output;

    Loop*               loop     = looper->loop;
    LooperSettings*     settings = looper->settings;

    if (loop_pos_before_end(loop, n_samples))
    {
        memcpy(output, &(loop->buffer[loop->pos]), n_samples * sizeof(float));
        loop->pos += n_samples;
    }
    //position is greater than loop length 
    //looping around to start as long as we have a loop
    else if (loop_exists(loop, n_samples))
    {
        memcpy(output, loop->buffer, n_samples * sizeof(float));
        loop->pos = n_samples;
    }
    else //no loop, output silence
    {
        memset(output, 0, n_samples * sizeof(float));
    }
}
