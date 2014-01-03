#include "minunit.h"
#include "looper.h"
#include <stdio.h>
#include <string.h>

#define N_FRAMES 64
#define RATE 48000.0

int tests_run = 0;

float in_buf[N_FRAMES];
float out_buf[N_FRAMES];

static Looper* setup_looper(void)
{
    Looper* instance = looper_new(N_FRAMES);
    looper_reset(instance);
    instance->input  = in_buf;
    instance->output = out_buf;
    return instance;
}

//Test default silence while and while not recording.
static char* test_record(void)
{

    Looper* instance = setup_looper();
    float test_var;
    for (int i = 0; i < N_FRAMES; i++)
    {
        in_buf[i]  = 0.1;
        out_buf[i] = 0.1;
    }

    instance->settings->requested_state = SlimMessage_Looper_State_RECORD;
    looper_run(instance, N_FRAMES);

    test_var = 0.1;
    for (int i = 0; i < N_FRAMES; i++)
    {
        mu_assert("test_record: not silent while recording", out_buf[i] == 0.0);
        mu_assert("test_record: changed input while recording", in_buf[i] == test_var);
    }

    instance->settings->requested_state = SlimMessage_Looper_State_PAUSE;
    looper_run(instance, N_FRAMES);

    for (int i = 0; i < N_FRAMES; i++)
    {
        mu_assert("test_record: not silent while paused", out_buf[i] == 0.0);
        mu_assert("test_record: changed input while paused", in_buf[i] == test_var);
    }

    instance->settings->requested_state = SlimMessage_Looper_State_PLAY;
    looper_run(instance, N_FRAMES);

    for (int i = 0; i < N_FRAMES; i++)
    {
        mu_assert("test_record: not playing back recording", out_buf[i] == test_var);
        mu_assert("test_record: changed input while playing", in_buf[i] == test_var);
    }

    looper_free(instance);

    return 0;
}

static char* test_overdub(void)
{
    Looper* instance = setup_looper();
    for (int i = 0; i < N_FRAMES; i++)
    {
        in_buf[i]  = 0.1;
        out_buf[i] = 0.1;
    }
    instance->settings->requested_state = SlimMessage_Looper_State_RECORD;
    looper_run(instance, N_FRAMES);
    instance->settings->requested_state = SlimMessage_Looper_State_OVERDUB;
    looper_run(instance, N_FRAMES);
    for (int i = 0; i < N_FRAMES; i++)
    {
        mu_assert("test_overdub: not playing back recording", out_buf[i] == in_buf[i]);
    }
    instance->settings->requested_state = SlimMessage_Looper_State_PLAY;
    looper_run(instance, N_FRAMES);
    float test_var = 0.2;
    for (int i = 0; i < N_FRAMES; i++)
    {
        mu_assert("test_overdub: not playing back overdub", out_buf[i] == test_var);
    }
    looper_free(instance);
    return 0;
}

static char* test_insert(void)
{
    Looper* instance = setup_looper();
    float test_buf[N_FRAMES];
    float test_var;
    instance->settings->requested_state = SlimMessage_Looper_State_RECORD;
    for (int i = 0; i < N_FRAMES; i++)
        in_buf[i]  = 0.1;
    looper_run(instance, N_FRAMES);

    instance->settings->requested_state = SlimMessage_Looper_State_RECORD;
    for (int i = 0; i < N_FRAMES; i++)
        in_buf[i]  = 0.2;
    looper_run(instance, N_FRAMES);

    instance->settings->requested_state = SlimMessage_Looper_State_PLAY;
    looper_run(instance, N_FRAMES);

    instance->settings->requested_state = SlimMessage_Looper_State_INSERT;
    for (int i = 0; i < N_FRAMES; i++)
        in_buf[i]  = 0.3;
    looper_run(instance, N_FRAMES);

    for (int i = 0; i < N_FRAMES; i++)
    {
        mu_assert("test_insert: not silent while inserting" , out_buf[i] == 0.0);
    }

    instance->settings->requested_state = SlimMessage_Looper_State_PLAY;
    looper_run(instance, N_FRAMES);

    test_var = 0.2;
    for (int i = 0; i < N_FRAMES; i++)
    {
        mu_assert("test_insert: not playing back second recording"
                 , out_buf[i] == test_var
                 );
    }

    instance->settings->requested_state = SlimMessage_Looper_State_PLAY;
    looper_run(instance, N_FRAMES);
    test_var = 0.1;
    for (int i = 0; i < N_FRAMES; i++)
    {
        mu_assert("test_insert: not playing back first recording", out_buf[i] == test_var);
    }

    instance->settings->requested_state = SlimMessage_Looper_State_PLAY;
    looper_run(instance, N_FRAMES);
    test_var = 0.3;
    for (int i = 0; i < N_FRAMES; i++)
    {
        mu_assert("test_insert: not playing back first recording", out_buf[i] == test_var);
    }
    looper_free(instance);
    return 0;
}

static char* test_replace(void)
{
    Looper* instance = setup_looper();
    looper_free(instance);
    return 0;
}

static char* all_tests() 
{
    mu_run_test(test_record);
    mu_run_test(test_overdub);
    mu_run_test(test_insert);
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

    return result != 0;
}
