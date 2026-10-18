#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void reverse_string(char *string)
{
    size_t left = 0;
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t right = strlen(string);

    while (left < right) {
        char temporary;

        --right;
        temporary = string[left];
        string[left] = string[right];
        string[right] = temporary;
        ++left;
    }
}

static void reverse_strings(char *strings[], size_t count)
{
    size_t index;

    if (strings == NULL) {
        return;
    }

    for (index = 0; index < count; ++index) {
        if (strings[index] != NULL) {
            reverse_string(strings[index]);
        }
    }
}

int main(void)
{
    const char *values[] = {
        "Hello",
        "Experienced C",
        "Safe programming",
        ""
    };
    const size_t count = sizeof(values) / sizeof(values[0]);
    char **strings = calloc(count, sizeof(*strings));
    size_t index;
    int result = EXIT_SUCCESS;

    if (strings == NULL) {
        fputs("Memory allocation failed.\n", stderr);
        return EXIT_FAILURE;
    }

    for (index = 0; index < count; ++index) {
        /* Possible weaknesses found:
         * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
         */
        size_t length = strlen(values[index]);

        strings[index] = malloc(length + 1U);
        if (strings[index] == NULL) {
            fputs("Memory allocation failed.\n", stderr);
            result = EXIT_FAILURE;
            break;
        }

        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(strings[index], values[index], length + 1U);
    }

    if (result == EXIT_SUCCESS) {
        reverse_strings(strings, count);

        for (index = 0; index < count; ++index) {
            if (printf("%s\n", strings[index]) < 0) {
                fputs("Output failure.\n", stderr);
                result = EXIT_FAILURE;
                break;
            }
        }
    }

    for (index = 0; index < count; ++index) {
        free(strings[index]);
    }
    free(strings);

    return result;
}