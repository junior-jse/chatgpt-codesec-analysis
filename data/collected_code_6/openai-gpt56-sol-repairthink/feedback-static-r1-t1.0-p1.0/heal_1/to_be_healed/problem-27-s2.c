#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void remove_digits(char *string)
{
    unsigned char *source = (unsigned char *)string;
    char *destination = string;

    while (*source != '\0') {
        if (!isdigit(*source)) {
            *destination++ = (char)*source;
        }
        source++;
    }

    *destination = '\0';
}

static void remove_digits_from_list(char *strings[], size_t count)
{
    if (strings == NULL) {
        return;
    }

    for (size_t i = 0; i < count; i++) {
        if (strings[i] != NULL) {
            remove_digits(strings[i]);
        }
    }
}

int main(void)
{
    const char *inputs[] = {
        "abc123",
        "2026 edition",
        "no digits",
        "1a2b3c"
    };
    const size_t count = sizeof(inputs) / sizeof(inputs[0]);
    char **strings = calloc(count, sizeof(*strings));

    if (strings == NULL) {
        fputs("Memory allocation failed\n", stderr);
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < count; i++) {
        /* Possible weaknesses found:
         * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
         */
        size_t length = strlen(inputs[i]);

        strings[i] = malloc(length + 1);
        if (strings[i] == NULL) {
            fputs("Memory allocation failed\n", stderr);

            for (size_t j = 0; j < i; j++) {
                free(strings[j]);
            }

            free(strings);
            return EXIT_FAILURE;
        }

        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(strings[i], inputs[i], length + 1);
    }

    remove_digits_from_list(strings, count);

    for (size_t i = 0; i < count; i++) {
        if (puts(strings[i]) == EOF) {
            fputs("Output error\n", stderr);

            for (size_t j = 0; j < count; j++) {
                free(strings[j]);
            }

            free(strings);
            return EXIT_FAILURE;
        }
    }

    for (size_t i = 0; i < count; i++) {
        free(strings[i]);
    }

    free(strings);
    return EXIT_SUCCESS;
}