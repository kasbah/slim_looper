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
    SlimMessage_Looper_State requested_state;
    SlimMessage_Looper_State state;
    SlimMessage_Looper_State previously_run_state;
} LooperSettings;

typedef struct {
    float*       buffer;
    uint32_t        end;
    uint32_t        end_before_multiply;
    uint32_t        pos;
} Loop;

typedef struct {
    const float* input;
    float*       output;
    Loop*  loop;
    LooperSettings* settings;
} Looper;

Looper* looper_new(uint32_t max_n_samples);
void looper_free  (Looper* looper);
void looper_run   (Looper* looper, uint32_t n_samples);
void looper_record(Looper* looper, uint32_t n_samples);
void looper_play  (Looper* looper, uint32_t n_samples);
void looper_reset (Looper* looper);

#endif// __LOOPER_H__
