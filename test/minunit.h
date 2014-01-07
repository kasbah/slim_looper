#ifndef __MINUNIT_H__
#define __MINUNIT_H__
#include <stdlib.h>
#include <string.h>

char *join(const char* s1, const char* s2)
{
    char* result = malloc(strlen(s1) + strlen(s2) + 1);

    if (result) // thanks @pmg
    {
        strcpy(result, s1);
        strcat(result, s2);
    }

    return result;
}

#define mu_assert(message, test) do { if (!(test)) return message; } while (0)
#define mu_run_test(test_name, test) do { char *message = test(); tests_run++; \
                               if (message) return join(test_name, message); } while (0)
extern int tests_run;

#endif// __MINUNIT_H__
