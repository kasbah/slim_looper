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

Looper* looper_new(size_t max_n_samples)
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
    free(looper->output);
    free(looper->loop->buffer);
    free(looper->loop);
    free(looper);
}

void looper_reset(Looper* looper)
{
    looper->loop->pos = 0;
    looper->loop->end = 0;
    looper->loop->end_before_extend = 0;
    looper->settings->state                 = SlimMessage_Looper_State_PAUSE;
    looper->settings->requested_state       = SlimMessage_Looper_State_PAUSE;
    looper->settings->previously_run_state  = SlimMessage_Looper_State_PAUSE;
}

void looper_run(Looper* looper, size_t n_samples)
{
    LooperSettings* settings = looper->settings;
    Loop*           loop     = looper->loop;
    settings->state = settings->requested_state;
    memset(looper->output, 0, n_samples * sizeof(float));
    switch(settings->state)
    {
        case SlimMessage_Looper_State_RECORD:
            //reset if we are not already running
            if (settings->previously_run_state != SlimMessage_Looper_State_RECORD)
            {
                loop->pos = 0;
                loop->end = 0;
            }
            record(looper, n_samples);
            break;
        case SlimMessage_Looper_State_OVERDUB:
            overdub(looper, n_samples);
            break;
        case SlimMessage_Looper_State_INSERT:
            insert(looper, n_samples);
            break;
        case SlimMessage_Looper_State_REPLACE:
            replace(looper, n_samples);
            break;
        case SlimMessage_Looper_State_EXTEND:
            if (settings->previously_run_state != SlimMessage_Looper_State_EXTEND)
            {
                loop->end_before_extend = loop->end;
                loop->pos_extend = loop->pos;
            }
            extend(looper, n_samples);
            break;
        case SlimMessage_Looper_State_PLAY:
            play(looper, n_samples);
            break;
        case SlimMessage_Looper_State_PAUSE:
        default:
            break;
    }
    settings->previously_run_state = settings->state;
}

static void record(Looper* looper, size_t n_samples)
{
    Loop* loop = looper->loop;
    memcpy( &(loop->buffer[loop->pos])
            , looper->input
            , n_samples * sizeof(float)
          );
    loop->pos += n_samples;
    loop->end += n_samples;
}

static void overdub(Looper* looper, size_t n_samples)
{
    Loop* loop = looper->loop;
    if (loop->end > 0)
    {
        if (loop->pos >= loop->end)
            loop->pos -= loop->end;
        memcpy( looper->output
                , &(loop->buffer[loop->pos])
                , n_samples * sizeof(float)
              );
        for (int i = 0; i < n_samples; i++)
        {
            //TODO: reduce gain to stop clipping 
            loop->buffer[loop->pos + i] += looper->input[i];
        }
        loop->pos += n_samples;
    }
}

static void insert(Looper* looper, size_t n_samples)
{
    Loop* loop = looper->loop;
    if (loop->end > 0)
    {
        if (loop->pos >= loop->end)
            loop->pos -= loop->end;
        //push the existing loop along by n_samples
        memmove( &(loop->buffer[loop->pos + n_samples])
                , &(loop->buffer[loop->pos])
                , (loop->end - loop->pos) * sizeof(float)
               );
        //fill the space with the input
        memcpy( &(loop->buffer[loop->pos])
                , looper->input
                , n_samples * sizeof(float)
              );
        loop->pos += n_samples;
        loop->end += n_samples;
    }
}

static void replace(Looper* looper, size_t n_samples)
{
    Loop* loop = looper->loop;
    if (loop->end > 0)
    {
        if (loop->pos >= loop->end)
            loop->pos -= loop->end;
        memcpy(&(loop->buffer[loop->pos])
                , looper->input
                , n_samples * sizeof(float)
              );
        loop->pos += n_samples;
    }
}
static void extend(Looper* looper, size_t n_samples)
{
    Loop* loop = looper->loop;
    if (loop->end >= 0) 
    {
        if (loop->pos_extend >= loop->end_before_extend)
            loop->pos_extend -= loop->end_before_extend;
        memcpy(looper->output
                , &(loop->buffer[loop->pos_extend])
                , n_samples * sizeof(float)
              );
        if ((loop->pos + n_samples) > (loop->end_before_extend)) 
        {
            //copy the current block
            memcpy( &(loop->buffer[loop->pos])
                    , &(loop->buffer[loop->pos_extend])
                    , n_samples * sizeof(float)
                  );
            loop->end += n_samples;
        }
        for (int i = 0; i < n_samples; i++)
        {   
            //TODO: reduce gain to stop clipping 
            loop->buffer[loop->pos + i] += looper->input[i];
        }
        loop->pos += n_samples;
        loop->pos_extend += n_samples;
    }
}

static void
play(Looper* looper, size_t n_samples)
{
    Loop* loop = looper->loop;
    if (loop->pos >= loop->end)
        loop->pos -= loop->end;
    memcpy(looper->output, &(loop->buffer[loop->pos]), n_samples * sizeof(float));
    loop->pos += n_samples;
}

