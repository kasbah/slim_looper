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
#ifndef __LOOPER_H__
#define __LOOPER_H__

#include <stdint.h>
#include "protocol/slim.pb.h"

#define LOOP_MAX_SAMPLES 19200000 

typedef struct {
    SlimMessage_Looper_State requested;
    SlimMessage_Looper_State current;
    SlimMessage_Looper_State previously_run;
} LooperState;

typedef struct {
    float feedback;
    float volume;
} LooperSettings;

typedef struct {
    float* buffer;
    size_t end;
    size_t pos;
    size_t end_before_extend;
    size_t pos_extend;
} Loop;

typedef struct {
    const float* input;
    float*       output;
    Loop*  loop;
    LooperState* state;
    LooperSettings* settings;
} Looper;

Looper* looper_new(size_t max_n_samples);
void looper_free  (Looper* looper);
void looper_run   (Looper* looper, size_t n_samples);
void looper_reset (Looper* looper);

static void record (Loop* loop, size_t n_samples, const float* const input);
static void overdub(Loop* loop, 
                    size_t n_samples, 
                    const float* const input, 
                    float* output,
                    float volume,
                    float feedback);
static void insert (Loop* loop, size_t n_samples, const float* const input);
static void extend (Loop* loop, size_t n_samples, 
                    const float* const input, 
                    float* output,
                    float volume,
                    float feedback);
static void play   (Loop* loop, size_t n_samples, float* output, float volume);
#endif// __LOOPER_H__
