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
#ifndef __SLIM_LV2_H__
#define __SLIM_LV2_H__

#include "slim.h"
#include "looper.h"

#include "lv2/lv2plug.in/ns/lv2core/lv2.h"
#include "lv2/lv2plug.in/ns/ext/atom/atom.h"
#include "lv2/lv2plug.in/ns/ext/midi/midi.h"
#include "lv2/lv2plug.in/ns/ext/atom/util.h"
#include "lv2/lv2plug.in/ns/ext/log/log.h"
#include "lv2/lv2plug.in/ns/ext/log/logger.h"
#include "lv2/lv2plug.in/ns/ext/buf-size/buf-size.h"

#define LOOPER_URI "https://github.com/kasbah/slim_looper.lv2"

typedef enum {
    PORT_INPUT        = 0,
    PORT_OUTPUT       = 1,
    PORT_CONTROL      = 2,
    PORT_RECORD_MODE  = 3,
    PORT_MIDI_IN      = 4
} PortIndex;

typedef struct {
    const float* input;
    float*       output;
    const float* control_input;
    const float* record_mode_input;
    LV2_Atom_Sequence* midi_input;
    LV2_Log_Log* log;
    LV2_Log_Logger logger;
    LV2_URID_Map* map;
    uint8_t midi_Event;
    uint32_t maxBlockLength;
    Slim* slim;
} SlimLV2;

LV2_SYMBOL_EXPORT
const LV2_Descriptor*
lv2_descriptor(uint32_t index);

#endif// __SLIM_LV2_H__
