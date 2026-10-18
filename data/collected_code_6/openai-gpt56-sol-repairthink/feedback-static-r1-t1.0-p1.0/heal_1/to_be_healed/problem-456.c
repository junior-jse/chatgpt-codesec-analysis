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

static int reverse_strings(char *strings[], size_t count)
{
    if (strings == NULL && count != 0U) {
        return -1;
    }

    for (size_t index = 0; index < count; ++index) {
        if (strings[index] == NULL) {
            return -1;
        }

        reverse_string(strings[index]);
    }

    return 0;
}

int main(void)
{
    char *strings[] = {
        "Hello",
        "World",
        "Secure C"
    };
    const size_t count = sizeof(strings) / sizeof(strings[0]);

    for (size_t index = 0; index < count; ++index) {
        /* Possible weaknesses found:
         * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
         */
        size_t length = strlen(strings[index]);
        char *copy = malloc(length + 1U);

        if (copy == NULL) {
            for (size_t cleanup = 0; cleanup < index; ++cleanup) {
                free(strings[cleanup]);
            }
            return EXIT_FAILURE;
        }

        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(copy, strings[index], length + 1U);
        strings[index] = copy;
    }

    if (reverse_strings(strings, count) != 0) {
        for (size_t index = 0; index < count; ++index) {
            free(strings[index]);
        }
        return EXIT_FAILURE;
    }

    for (size_t index = 0; index < count; ++index) {
        if (puts(strings[index]) == EOF) {
            for (size_t cleanup = 0; cleanup < count; ++cleanup) {
                free(strings[cleanup]);
            }
            return EXIT_FAILURE;
        }
    }

    for (size_t index = 0; index < count; ++index) {
        free(strings[index]);
    }

    return EXIT_SUCCESS;
}