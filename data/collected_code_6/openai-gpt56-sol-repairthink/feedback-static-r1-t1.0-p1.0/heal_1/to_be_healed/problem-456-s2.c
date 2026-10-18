#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int reverse_string(char *string)
{
    size_t left;
    size_t right;

    if (string == NULL) {
        return -1;
    }

    left = 0;
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    right = strlen(string);

    if (right == 0) {
        return 0;
    }

    --right;

    while (left < right) {
        char temp = string[left];
        string[left] = string[right];
        string[right] = temp;
        ++left;
        --right;
    }

    return 0;
}

static int reverse_strings(char *strings[], size_t count)
{
    size_t i;

    if (strings == NULL && count != 0) {
        return -1;
    }

    for (i = 0; i < count; ++i) {
        if (reverse_string(strings[i]) != 0) {
            return -1;
        }
    }

    return 0;
}

int main(void)
{
    char first[] = "hello";
    char second[] = "world";
    char third[] = "C programming";
    char *strings[] = { first, second, third };
    const size_t count = sizeof(strings) / sizeof(strings[0]);
    size_t i;

    if (reverse_strings(strings, count) != 0) {
        fputs("Failed to reverse strings.\n", stderr);
        return EXIT_FAILURE;
    }

    for (i = 0; i < count; ++i) {
        if (puts(strings[i]) == EOF) {
            fputs("Failed to write output.\n", stderr);
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}