#include "minunit.h"
#include "looper.h"
#include <stdio.h>
#include <string.h>
#include "lv2/lv2plug.in/ns/ext/atom/forge.h"
#include "lv2/lv2plug.in/ns/ext/midi/midi.h"

#define N_FRAMES 64
#define RATE 48000.0

int tests_run = 0;

LV2_Descriptor* looper;
LV2_Handle instance;
float in_buf[N_FRAMES];
float out_buf[N_FRAMES];
float control;
float record_mode;
uint8_t port_buf[64];

/* Test that instantiate returns a valid pointer.
 * Mainly we are setting everything up here.  
 */
static char* test_instantiate()
{
    looper = lv2_descriptor(0);
    instance = looper->instantiate(looper, 0.0, NULL, NULL);
    mu_assert("test_instantiate: instance is NULL", instance != NULL);
    looper->connect_port(instance, PORT_INPUT,             in_buf);
    looper->connect_port(instance, PORT_OUTPUT,           out_buf);
    looper->connect_port(instance, PORT_CONTROL,         &control);
    looper->connect_port(instance, PORT_RECORD_MODE, &record_mode);
    looper->connect_port(instance, PORT_ATOM,             port_buf);
    looper->activate(instance);
    return 0;
}

//Test default silence while and while not recording.
static char* test_silence()
{
    float test_buf[N_FRAMES];
    for (int i = 0; i < N_FRAMES; i++)
    {
        in_buf[i]  = 0.1;
        out_buf[i] = 0.1;
        test_buf[i]= 0.1;
    }
    control = PAUSED; 
    record_mode = MODE_NEW; 

    looper->run(instance, N_FRAMES);

    for (int i = 0; i < N_FRAMES; i++)
    {
        mu_assert("test_silence: not silent when paused", out_buf[i] == 0.0);
        mu_assert("test_silence: changed input", in_buf[i] == test_buf[i]);
    }

    control = RECORDING; 
    looper->run(instance, N_FRAMES);

    for (int i = 0; i < N_FRAMES; i++)
    {
        mu_assert("test_silence: not silent when recording", out_buf[i] == 0.0);
        mu_assert("test_silence: changed input", in_buf[i] == test_buf[i]);
    }

    return 0;
}

//Test record works.
//static char* test_record()
//{
//
//}

//Test atom port
static char* test_atom()
{
    LV2_Atom_Forge forge;
    LV2_Atom_Forge_Frame frame;
    LV2_URID_Map map;

    /* the data that we want to send */
    uint8_t raw_midi[3];
    raw_midi[0] = 0x90;
    raw_midi[1] = 127;
    raw_midi[2] = 127;

    /* and the length of the data */
    size_t  midi_message_length = 3;
 
    /* Message payload header */
    LV2_Atom midiatom;
    midiatom.type = map.map(map.handle, LV2_MIDI__MidiEvent);
    midiatom.size = midi_message_length;

    lv2_atom_forge_init(&forge, &map);

    /* buffer used to forge the message in */
    uint8_t atom_buf[64];
    /* make forge use this buffer for now */
    lv2_atom_forge_set_buffer(&forge, atom_buf, sizeof(atom_buf));
 
    /* .. then we need to specify what this message is about and how long the
     * payload is: add the header */
    lv2_atom_forge_raw(&forge, &midiatom, sizeof(LV2_Atom));
    /* now we we can add the actual data */
    lv2_atom_forge_raw(&forge, raw_midi, midi_message_length);
    /* finally pad the message to make it 32bit aligned */
    lv2_atom_forge_pad(&forge, sizeof(LV2_Atom) + midi_message_length);

    looper->run(instance, N_FRAMES);

    mu_assert("yo", 1);

    return 0;
    
}



static char* all_tests() 
{
    mu_run_test(test_instantiate);
    mu_run_test(test_silence);
    mu_run_test(test_atom);
    return 0;
}

int main(int argc, char **argv) {
    char *result = all_tests();
    if (result != 0) {
        printf("%s\n", result);
    }
    else {
        printf("ALL TESTS PASSED\n");
    }
    printf("Tests run: %d\n", tests_run);

    looper->deactivate(instance);
    return result != 0;
}
