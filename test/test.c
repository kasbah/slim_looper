#include "minunit.h"
#include "lv2/lv2plug.in/ns/lv2core/lv2.h"
#include "looper.h"
#include <stdio.h>
#include <string.h>

#define N_FRAMES 64
#define RATE 48000.0

int tests_run = 0;

LV2_Descriptor* looper;
LV2_Handle instance;
float in_buf[N_FRAMES];
float out_buf[N_FRAMES];
LV2_Atom_Sequence midi_buf[N_FRAMES];
float control;
float record_mode;

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
    looper->connect_port(instance, PORT_MIDI_IN, midi_buf);
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



static char* all_tests() 
{
    mu_run_test(test_instantiate);
    mu_run_test(test_silence);
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
