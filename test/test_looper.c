#include "minunit.h"
#include "looper.h"
#include <stdio.h>
#include <string.h>

#define N_FRAMES 2 

#define VERBOSE 0
#if VERBOSE
#define assert_all(message, buffer, value) do {\
    float test_var = value;\
    for (int i = 0; i < N_FRAMES; i++) {printf("sample: %i value:%f\r\n",i, buffer[i]);mu_assert(message, buffer[i] == test_var);}\
} while (0)
#else
#define assert_all(message, buffer, value) do {\
    float test_var = value;\
    for (int i = 0; i < N_FRAMES; i++) {mu_assert(message, buffer[i] == test_var);}\
} while (0)
#endif

int tests_run = 0;

float input[N_FRAMES];
float output[N_FRAMES];

static Looper* setup_looper(void)
{
    Looper* instance = looper_new(N_FRAMES);
    looper_reset(instance);
    instance->input  = input;
    instance->output = output;
    return instance;
}

static inline void set_all(float* buffer, float value)
{
    for (int i = 0; i < N_FRAMES; i++)
        buffer[i] = value;
}

static char* test_record(void)
{
    Looper* instance = setup_looper();
    set_all(input, 0.1);
    set_all(output, 0.0);
    instance->settings->requested_state = SlimMessage_Looper_State_RECORD;
    looper_run(instance, N_FRAMES);
    assert_all("silent while recording", output, 0.0);
    assert_all("keeping input unchanged while recording", input, 0.1);
    instance->settings->requested_state = SlimMessage_Looper_State_PAUSE;
    looper_run(instance, N_FRAMES);
    assert_all("silent while paused", output, 0.0);
    assert_all("keeping input unchanged while paused", input, 0.1);
    instance->settings->requested_state = SlimMessage_Looper_State_PLAY;
    looper_run(instance, N_FRAMES);
    assert_all("playing back recording", output, 0.1);
    assert_all("keeping input unchanged while playing", output, 0.1);
    looper_free(instance);
    return 0;
}

static char* test_overdub(void)
{
    Looper* instance = setup_looper();
    set_all(input, 0.1);
    set_all(output, 0.2);
    instance->settings->requested_state = SlimMessage_Looper_State_RECORD;
    looper_run(instance, N_FRAMES);
    instance->settings->requested_state = SlimMessage_Looper_State_OVERDUB;
    looper_run(instance, N_FRAMES);
    assert_all("playing back recording", output, 0.1);
    instance->settings->requested_state = SlimMessage_Looper_State_PLAY;
    looper_run(instance, N_FRAMES);
    assert_all("playing back overdub", output, 0.2);
    looper_free(instance);
    return 0;
}

static char* test_insert(void)
{
    Looper* instance = setup_looper();
    set_all(input, 0.1);
    instance->settings->requested_state = SlimMessage_Looper_State_RECORD;
    looper_run(instance, N_FRAMES);
    set_all(input, 0.2);
    looper_run(instance, N_FRAMES);
    instance->settings->requested_state = SlimMessage_Looper_State_PLAY;
    looper_run(instance, N_FRAMES);
    instance->settings->requested_state = SlimMessage_Looper_State_INSERT;
    set_all(input, 0.3);
    looper_run(instance, N_FRAMES);
    assert_all("silent while inserting", output, 0.0);
    instance->settings->requested_state = SlimMessage_Looper_State_PLAY;
    looper_run(instance, N_FRAMES);
    assert_all("playing back second recording", output, 0.2);
    looper_run(instance, N_FRAMES);
    assert_all("playing back first recording", output, 0.1);
    looper_run(instance, N_FRAMES);
    assert_all("playing back inserted recording", output, 0.3);
    instance->settings->requested_state = SlimMessage_Looper_State_PLAY;
    looper_run(instance, N_FRAMES);
    assert_all("playing back second recording after insert", output, 0.2);
    looper_free(instance);
    return 0;
}

static char* test_replace(void)
{
    Looper* instance = setup_looper();
    set_all(input, 0.1);
    instance->settings->requested_state = SlimMessage_Looper_State_RECORD;
    looper_run(instance, N_FRAMES);
    set_all(input, 0.2);
    looper_run(instance, N_FRAMES);
    instance->settings->requested_state = SlimMessage_Looper_State_REPLACE;
    set_all(input, 0.3);
    looper_run(instance, N_FRAMES);
    assert_all("silent while replacing", output, 0.0);
    instance->settings->requested_state = SlimMessage_Looper_State_PLAY;
    looper_run(instance, N_FRAMES);
    assert_all("playing second recording", output, 0.2);
    looper_run(instance, N_FRAMES);
    assert_all("playing back replaced", output, 0.3);
    looper_free(instance);
    return 0;
}
static char* test_extend1(void)
{
    Looper* instance = setup_looper();
    set_all(input, 0.1);
    set_all(output, 0.0);
    instance->settings->requested_state = SlimMessage_Looper_State_RECORD;
    looper_run(instance, N_FRAMES);
    set_all(input, 0.2);
    looper_run(instance, N_FRAMES);

    set_all(input, 0.05);
    instance->settings->requested_state = SlimMessage_Looper_State_EXTEND;
    looper_run(instance, N_FRAMES);
    assert_all("playing back first recording (1)", output, 0.1);
    looper_run(instance, N_FRAMES);
    assert_all("playing back second recording (2)", output, 0.2);

    set_all(input, 0.1);
    looper_run(instance, N_FRAMES);
    assert_all("playing back first recording (3)", output, 0.1);
    looper_run(instance, N_FRAMES);
    assert_all("playing back second recording (4)", output, 0.2);

    instance->settings->requested_state = SlimMessage_Looper_State_PLAY;
    looper_run(instance, N_FRAMES);
    assert_all("playing back first recording after multiply (5)", output, 0.1);
    looper_run(instance, N_FRAMES);
    assert_all("playing back second recording after multiply (6)", output, 0.2);

    looper_run(instance, N_FRAMES);
    assert_all("playing back first multiply (7)", output, 0.15);
    looper_run(instance, N_FRAMES);
    assert_all("playing back second multiply (8)", output, 0.25);

    looper_run(instance, N_FRAMES);
    assert_all("playing back third multiply (9)", output, 0.2);
    looper_run(instance, N_FRAMES);
    assert_all("playing back fourth multiply (10)", output, 0.3);

    looper_free(instance);
    return 0;
}
static char* test_extend2(void)
{
    Looper* instance = setup_looper();
    set_all(input, 0.1);
    set_all(output, 0.0);
    instance->settings->requested_state = SlimMessage_Looper_State_RECORD;
    looper_run(instance, N_FRAMES);
    set_all(input, 0.2);
    looper_run(instance, N_FRAMES);
    set_all(input, 0.05);
    instance->settings->requested_state = SlimMessage_Looper_State_EXTEND;
    looper_run(instance, N_FRAMES);
    assert_all("playing back first recording (1)", output, 0.1);
    instance->settings->requested_state = SlimMessage_Looper_State_PLAY;
    looper_run(instance, N_FRAMES);
    assert_all("playing back first recording after multiply (2)", output, 0.1);
    looper_run(instance, N_FRAMES);
    assert_all("playing back second recording after multiply (3)", output, 0.2);
    looper_run(instance, N_FRAMES);
    assert_all("playing extended overdub (4)", output, 0.15);
    looper_run(instance, N_FRAMES);
    assert_all("playing back first recording after multiply (5)", output, 0.1);
    looper_run(instance, N_FRAMES);
    assert_all("playing back second recording after multiply (6)", output, 0.2);
    looper_free(instance);
    return 0;
}

static char* all_tests() 
{
    mu_run_test("test_record: NOT ", test_record);
    mu_run_test("test_overdub: NOT ", test_overdub);
    mu_run_test("test_insert: NOT ", test_insert);
    mu_run_test("test_replace: NOT ", test_replace);
    mu_run_test("test_extend1: NOT ", test_extend1);
    mu_run_test("test_extend2: NOT ", test_extend2);
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
