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
#ifndef __LOOPER_H__
#define __LOOPER_H__

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

#endif// __LOOPER_H__
