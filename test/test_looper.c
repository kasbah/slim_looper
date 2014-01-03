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
    Looper* looper = looper_new(N_FRAMES);
    looper_reset(looper);
    looper->input  = in_buf;
    looper->output = out_buf;
    return looper;
}

//Test default silence while and while not recording.
static char* test_record(void)
{

    Looper* looper = setup_looper();
    float test_buf[N_FRAMES];
    for (int i = 0; i < N_FRAMES; i++)
    {
        in_buf[i]  = 0.1;
        out_buf[i] = 0.1;
        test_buf[i]= 0.1;
    }

    looper->settings->requested_state = SlimMessage_Looper_State_RECORD;
    looper_run(looper, N_FRAMES);

    for (int i = 0; i < N_FRAMES; i++)
    {
        mu_assert("test_record: not silent while recording", out_buf[i] == 0.0);
        mu_assert("test_record: changed input while recording", in_buf[i] == test_buf[i]);
    }

    looper->settings->requested_state = SlimMessage_Looper_State_PAUSE;
    looper_run(looper, N_FRAMES);

    for (int i = 0; i < N_FRAMES; i++)
    {
        mu_assert("test_record: not silent while paused", out_buf[i] == 0.0);
        mu_assert("test_record: changed input while paused", in_buf[i] == test_buf[i]);
    }

    looper->settings->requested_state = SlimMessage_Looper_State_PLAY;
    looper_run(looper, N_FRAMES);

    for (int i = 0; i < N_FRAMES; i++)
    {
        mu_assert("test_record: not playing back recording", out_buf[i] == test_buf[i]);
        mu_assert("test_record: changed input while playing", in_buf[i] == test_buf[i]);
    }

    looper_free(looper);

    return 0;
}

static char* test_overdub(void)
{
    Looper* looper = setup_looper();
    looper_free(looper);
    return 0;
}

static char* all_tests() 
{
    mu_run_test(test_record);
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
