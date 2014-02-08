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
    looper->loop->beg = calloc(LOOP_MAX_SAMPLES, sizeof(float));
    looper->state        = (LooperState*)malloc(sizeof(LooperState));
    looper->settings     = (LooperSettings*)malloc(sizeof(LooperSettings));
    return looper;
}

void looper_free(Looper* looper)
{
    free(looper->state);
    free(looper->settings);
    free(looper->output);
    free(looper->loop->beg);
    free(looper->loop);
    free(looper);
}

void looper_reset(Looper* looper)
{
    looper->loop->pos               = looper->loop->beg;
    looper->loop->end               = looper->loop->beg;

    looper->state->current         = SlimMessage_Looper_State_NONE;
    looper->state->requested       = SlimMessage_Looper_State_NONE;
    looper->state->previously_run  = SlimMessage_Looper_State_NONE;

    looper->settings->feedback = 1.0;
    looper->settings->volume   = 1.0;
}

void looper_run(Looper* looper, size_t n_samples)
{
    LooperState* state        = looper->state;
    Loop*        loop         = looper->loop;
    LooperSettings* settings  = looper->settings;
    switch(state->requested)
    {
        case SlimMessage_Looper_State_RECORD:
            state->current = state->requested;
            break;
        case SlimMessage_Looper_State_OVERDUB:
        case SlimMessage_Looper_State_INSERT:
        case SlimMessage_Looper_State_EXTEND:
        case SlimMessage_Looper_State_PLAY:
        case SlimMessage_Looper_State_PAUSE:
            if((loop->end - loop->beg) > 0)
            {
                state->current = state->requested;
            }
            break;
        case SlimMessage_Looper_State_NONE:
        default:
            break;
    }
    memset(looper->output, 0, n_samples * sizeof(float));
    switch(state->current)
    {
        case SlimMessage_Looper_State_RECORD:
            //reset if we are not already running
            if (state->previously_run != SlimMessage_Looper_State_RECORD)
            {
                loop->pos = loop->beg;
                loop->end = loop->beg;
            }
            record(loop, n_samples, looper->input);
            loop->pos += n_samples;
            break;
        case SlimMessage_Looper_State_OVERDUB:
            loop_around(loop);
            play   (loop, n_samples, looper->output, settings->volume);
            overdub(loop, n_samples, looper->input, settings->feedback);
            loop->pos += n_samples;
            break;
        case SlimMessage_Looper_State_INSERT:
            loop_around(loop);
            insert(loop, n_samples);
            record(loop, n_samples, looper->input);
            loop->pos += n_samples;
            break;
        case SlimMessage_Looper_State_EXTEND:
            if (state->previously_run != SlimMessage_Looper_State_EXTEND)
            {
                loop->end_extend_source = loop->end;
                loop->pos_extend_source = loop->pos;
            }
            extend (loop, n_samples);
            play   (loop, n_samples, looper->output, settings->volume);
            overdub(loop, n_samples, looper->input, settings->feedback);
            loop->pos += n_samples;
            break;
        case SlimMessage_Looper_State_PLAY:
            loop_around(loop);
            play(loop, n_samples, looper->output, settings->volume);
            loop->pos += n_samples;
            break;
        case SlimMessage_Looper_State_PAUSE:
        case SlimMessage_Looper_State_NONE:
        default:
            break;
    }
    state->previously_run = state->current;
}

static void loop_around(Loop* loop)
{
    if (loop->pos >= loop->end)
        loop->pos -= (loop->end - loop->beg);
}

static void record(Loop* loop, size_t n_samples, const float* const input)
{
    memcpy(loop->pos, input, n_samples * sizeof(float) );
    loop->end += n_samples;
}

static void overdub(Loop* loop,
                    size_t n_samples,
                    const float* const input,
                    float feedback)
{
    for (int i = 0; i < n_samples; i++)
    {
        //TODO: reduce feedback to stop clipping
        loop->pos[i] *= feedback;
        loop->pos[i] += input[i];
    }
}

static void insert(Loop* loop, size_t n_samples)
{
    //push the existing loop along by n_samples
    memmove(loop->pos + n_samples,
            loop->pos,
            (loop->end - loop->pos) * sizeof(float) );
}

static void extend(Loop* loop, size_t n_samples)
{
    if (loop->pos_extend_source >= loop->end_extend_source)
        loop->pos_extend_source -= (loop->end_extend_source - loop->beg);
    if ((loop->pos + n_samples) > (loop->end_extend_source))
    {
        //copy the current block
        memcpy(loop->pos,
               loop->pos_extend_source,
               n_samples * sizeof(float) );
        loop->end += n_samples;
    }
    loop->pos_extend_source += n_samples;
}

static void play(Loop* loop, size_t n_samples, float* output, float volume)
{
    for (int i = 0; i < n_samples; i++)
    {
        output[i] = loop->pos[i] * volume;
    }
}

