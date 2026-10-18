#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void reverse_string(char *string)
{
    size_t left = 0;
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
        size_t length = strlen(strings[index]);
        char *copy = malloc(length + 1U);

        if (copy == NULL) {
            for (size_t cleanup = 0; cleanup < index; ++cleanup) {
                free(strings[cleanup]);
            }
            return EXIT_FAILURE;
        }

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