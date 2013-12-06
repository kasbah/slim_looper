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

#define LOOP_MAX_SAMPLES 19200000 

typedef enum {
    PAUSED    = 0,
    PLAYING   = 1,
    RECORDING = 2
} LooperState;

typedef enum {
    MODE_NEW     = 0,
    MODE_OVERDUB = 1,
    MODE_INSERT  = 2,
    MODE_REPLACE = 3
} LooperRecordMode;

typedef struct {
    LooperRecordMode record_mode;
} LooperSettings;


typedef struct {
    float*       buffer;
    unsigned       end;
    unsigned       pos;
} Loop;

typedef struct {
    const float* input;
    float*       output;
    Loop*  loop;
    LooperSettings* settings;
    LooperState state;
    LooperState previous_state;
} Looper;


Looper* looper_new(void);

void looper_free(Looper* looper);

void
looper_record(Looper* looper, uint32_t n_samples);

void
looper_play(Looper* looper, uint32_t n_samples);


#endif// __LOOPER_H__
