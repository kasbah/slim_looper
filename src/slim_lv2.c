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

#include "slim_lv2.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static LV2_Handle
instantiate(const LV2_Descriptor*     descriptor,
            double                    rate,
            const char*               bundle_path,
            const LV2_Feature* const* features)
{
    SlimLV2* self = (SlimLV2*)malloc(sizeof(SlimLV2));
    LV2_Options_Option* options = NULL;
    // Get host features
    if (features)
    {
        for (int i = 0; features[i]; ++i) {
            if (!strcmp(features[i]->URI, LV2_URID__map)) {
                self->map = (LV2_URID_Map*)features[i]->data;
            } 
            //else if (!strcmp(features[i]->URI, LV2_WORKER__schedule)) {
            //    self->schedule = (LV2_Worker_Schedule*)features[i]->data;
            //} 
            else if (!strcmp(features[i]->URI, LV2_LOG__log)) {
                self->log = (LV2_Log_Log*)features[i]->data;
            }
            else if (!strcmp(features[i]->URI, LV2_OPTIONS__options)) {
                options = (LV2_Options_Option*)features[i]->data;
            }
            //else if (!strcmp(features[i]->URI, LV2_BUF_SIZE__maxBlockLength)) {
            //    self->maxBlockLength = *((uint32_t*)(features[i]->data));
            //}
        }
    }

    if (!self->map) 
    {
        lv2_log_error(&self->logger, "Missing feature urid:map\n");
        goto fail;
    } 
    else if (!options)
    {
        lv2_log_error(&self->logger, "Missing feature ext:options\n");
        goto fail;
    }
    //else if (!self->schedule) {
    //    lv2_log_error(&self->logger, "Missing feature work:schedule\n");
    //    goto fail;
    //}

    self->midi_Event = self->map->map(self->map->handle, LV2_MIDI__MidiEvent);
    LV2_URID bufsz_max = self->map->map(self->map->handle, LV2_BUF_SIZE__maxBlockLength);
    LV2_URID atom_Int = self->map->map(self->map->handle, LV2_ATOM__Int);
    for (const LV2_Options_Option* o = options; o->key; ++o) 
    {
        if (o->context == LV2_OPTIONS_INSTANCE &&
                o->key == bufsz_max &&
                o->type == atom_Int) 
        {
            self->maxBlockLength = *(const uint32_t*)o->value;
        }
    }
    if (self->maxBlockLength == 0) 
    {
       lv2_log_error(&self->logger, "No maximum buffer size given.\n");
       goto fail;
    }

    self->slim = slim_new(1, self->maxBlockLength);

    return (LV2_Handle)self;
fail:
    slim_free(self->slim);
    free(self);
    return NULL;
}

static void
connect_port(LV2_Handle instance,
             uint32_t   port,
             void*      data)
{
    SlimLV2* self = (SlimLV2*)instance;

    switch ((PortIndex)port) {
    case PORT_INPUT:
        for (int i = 0; i < (self->slim->n_loopers); i++)
        {
            self->slim->looper_array[i]->input = (const float*)data;
        }
        break;
    case PORT_OUTPUT:
        self->output = (float*)data;
        self->slim->output = self->output;
        break;
    case PORT_CONTROL:
        self->control_input = (const float*)data;
        break;
    case PORT_RECORD_MODE:
        self->record_mode_input = (const float*)data;
        break;
    //case PORT_MIDI_IN:
    //    self->midi_input = (LV2_Atom_Sequence*) data;
    //    break;
    }
}

static void
activate(LV2_Handle instance)
{
    SlimLV2* self = (SlimLV2*)instance;
    slim_activate(self->slim);
}

/** Process a block of audio (audio thread, must be RT safe). */
static void
run(LV2_Handle instance, uint32_t n_samples)
{
    SlimLV2*        self     = (SlimLV2*)instance;

    self->slim->looper_array[0]->settings->record_mode  = (LooperRecordMode)(*(self->record_mode_input));
    self->slim->looper_array[0]->state                  = (LooperState)(*(self->control_input));

    //LV2_ATOM_SEQUENCE_FOREACH(self->midi_input, ev) 
    //{
    //    if (ev->body.type == self->midi_Event) {
    //        lv2_log_trace(&self->logger, "event: %u\r\n", ev->time.frames);
    //        lv2_log_trace(&self->logger, "event size: %u\r\n", ev->body.size);
    //    }
    //}

    slim_run(self->slim, n_samples);

}

static void
deactivate(LV2_Handle instance)
{
}

static void
cleanup(LV2_Handle instance)
{
    SlimLV2* self = (SlimLV2*)instance;
    slim_free(self->slim);
    free(self);
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

