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
    looper->loop->end_before_mult = 0;
    looper->settings->state                 = SlimMessage_Looper_State_PAUSE;
    looper->settings->requested_state       = SlimMessage_Looper_State_PAUSE;
    looper->settings->previously_run_state  = SlimMessage_Looper_State_PAUSE;
}

void looper_run(Looper* looper, uint32_t n_samples)
{
    LooperSettings* settings = looper->settings;
    settings->state = settings->requested_state;
    memset(looper->output, 0, n_samples * sizeof(float));
    switch(settings->state)
    {
        case SlimMessage_Looper_State_RECORD:
        case SlimMessage_Looper_State_OVERDUB:
        case SlimMessage_Looper_State_INSERT:
        case SlimMessage_Looper_State_REPLACE:
        case SlimMessage_Looper_State_MULTIPLY:
        case SlimMessage_Looper_State_EXTEND:
            looper_record(looper, n_samples);
            break;
        case SlimMessage_Looper_State_PLAY:
            looper_play(looper, n_samples);
            break;
        case SlimMessage_Looper_State_PAUSE:
        default:
            break;
    }
    settings->previously_run_state = settings->state;
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

static void
looper_record(Looper* looper, uint32_t n_samples)
{
    const float* const input  = looper->input;
    float* const       output = looper->output;

    Loop*               loop     = looper->loop;
    LooperSettings*     settings = looper->settings;

    switch(settings->state) 
    {
        case SlimMessage_Looper_State_RECORD:
            //reset if we are not already running
            if (settings->previously_run_state != SlimMessage_Looper_State_RECORD)
            {
                loop->pos = 0;
                loop->end = 0;
            }
            memcpy( &(loop->buffer[loop->pos])
                  , input
                  , n_samples * sizeof(float)
                  );
            loop->pos += n_samples;
            loop->end += n_samples;
            break;

        case SlimMessage_Looper_State_OVERDUB:
            if (loop_pos_before_end(loop, n_samples)) 
            {
                memcpy( output
                      , &(loop->buffer[loop->pos])
                      , n_samples * sizeof(float)
                      );
                for (int i = 0; i < n_samples; i++)
                {
                    //TODO: reduce gain to stop clipping 
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
                {
                    //TODO: reduce gain to stop clipping 
                    loop->buffer[i] += input[i];
                }
                loop->pos = n_samples;
            }
            break;

        case SlimMessage_Looper_State_INSERT:
            if (loop_exists(loop, n_samples))
            {
                //push the existing loop along by n_samples
                memmove( &(loop->buffer[loop->pos + n_samples])
                       , &(loop->buffer[loop->pos])
                       , (loop->end - loop->pos) * sizeof(float)
                       );
                //fill the space with the input
                memcpy( &(loop->buffer[loop->pos])
                      , input
                      , n_samples * sizeof(float)
                      );
                loop->pos += n_samples;
                loop->end += n_samples;
            }
            break;

        case SlimMessage_Looper_State_REPLACE:
            if (loop_pos_before_end(loop, n_samples)) 
            {
                memcpy(&(loop->buffer[loop->pos])
                      , input
                      , n_samples * sizeof(float)
                      );
                loop->pos += n_samples;
            }
            //position is greater than loop length 
            //looping around to start as long as we have a loop
            else if (loop_exists(loop, n_samples))
            {
                memcpy(loop->buffer, input, n_samples * sizeof(float));
                loop->pos = n_samples;
            }
            break;
        case SlimMessage_Looper_State_MULTIPLY:
            if (settings->previously_run_state != SlimMessage_Looper_State_MULTIPLY)
            {
                loop->end_before_mult = loop->end;
            }
            if (loop_pos_before_end(loop, n_samples)) 
            {
                memcpy( output
                      , &(loop->buffer[loop->pos])
                      , n_samples * sizeof(float)
                      );
                for (int i = 0; i < n_samples; i++)
                {   
                    //TODO: reduce gain to stop clipping 
                    loop->buffer[loop->pos + i] += input[i];
                }
                loop->pos += n_samples;
            }
            //position is greater than loop length. as long as we have a loop
            //increase loop length by making a copy of the original loop before 
            //multiply began 
            else if (loop_exists(loop, n_samples)) 
            {
                //output the exisiting loop from the beginning
                memcpy(output, loop->buffer, n_samples * sizeof(float));
                //copy the whole loop
                memcpy( &(loop->buffer[loop->end])
                      , loop->buffer
                      , loop->end_before_mult * sizeof(float)
                      );
                //add input to the loop
                for (int i = 0; i < n_samples; i++)
                {
                    //TODO: reduce gain to stop clipping 
                    loop->buffer[loop->pos + i] += input[i];
                }
                loop->pos += n_samples;
                loop->end += loop->end_before_mult;
            }
            break;
        case SlimMessage_Looper_State_EXTEND:
            if (settings->previously_run_state != SlimMessage_Looper_State_EXTEND)
            {
                loop->end_before_mult = loop->end;
                loop->pos_extend = loop->pos;
            }
            if (loop->pos_extend >= loop->end_before_mult)
                loop->pos_extend -= loop->end_before_mult;
            if ((loop->pos + n_samples) <= (loop->end_before_mult)) 
            {
                memcpy( output
                      , &(loop->buffer[loop->pos])
                      , n_samples * sizeof(float)
                      );
                for (int i = 0; i < n_samples; i++)
                {   
                    //TODO: reduce gain to stop clipping 
                    loop->buffer[loop->pos + i] += input[i];
                }
            }
            else if (loop_exists(loop, n_samples)) 
            {
                //output the exisiting loop from the beginning
                memcpy(output, &(loop->buffer[loop->pos_extend]), n_samples * sizeof(float));
                //copy the current block
                memcpy( &(loop->buffer[loop->pos])
                      , &(loop->buffer[loop->pos_extend])
                      , n_samples * sizeof(float)
                      );
                //add input to the loop
                for (int i = 0; i < n_samples; i++)
                {
                    //TODO: reduce gain to stop clipping 
                    loop->buffer[loop->pos + i] += input[i];
                }
                loop->end += n_samples;
            }
            loop->pos += n_samples;
            loop->pos_extend += n_samples;

        default:
            break;
    }
}

static void
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
}
