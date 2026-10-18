#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_STRING_LENGTH 1024
#define MAX_INPUTS 16

void remove_digits(char **list, size_t size, size_t buffer_size) {
    if (list == NULL || size == 0 || buffer_size < 2) {
        return;
    }

    for (size_t i = 0; i < size; i++) {
        if (list[i] == NULL) {
            continue;
        }

        size_t write = 0;

        /* Possible weaknesses found:
         * Flawfinder read: Check buffer boundaries if used in a loop including recursive loops (CWE-120, CWE-20). (risk 1, buffer)
         */
        for (size_t read = 0; read < buffer_size - 1 && list[i][read] != '\0'; read++) {
            /* Possible weaknesses found:
             * Flawfinder read: Check buffer boundaries if used in a loop including recursive loops (CWE-120, CWE-20). (risk 1, buffer)
             */
            unsigned char ch = (unsigned char)list[i][read];

            if (!isdigit(ch)) {
                if (write >= buffer_size - 1) {
                    break;
                }
                list[i][write] = (char)ch;
                write++;
            }
        }

        if (write < buffer_size) {
            list[i][write] = '\0';
        } else {
            list[i][buffer_size - 1] = '\0';
        }
    }
}

int main(void) {
    const char *inputs[] = {
        "hello123world",
        "abc456def",
        "no_digits_here",
        "12345"
    };
    const size_t size = sizeof(inputs) / sizeof(inputs[0]);

    if (size == 0 || size > MAX_INPUTS) {
        return EXIT_FAILURE;
    }

    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char *strings[MAX_INPUTS];
    for (size_t i = 0; i < MAX_INPUTS; i++) {
        strings[i] = NULL;
    }

    for (size_t i = 0; i < size && i < MAX_INPUTS; i++) {
        if (inputs[i] == NULL) {
            for (size_t j = 0; j < i; j++) {
                free(strings[j]);
                strings[j] = NULL;
            }
            return EXIT_FAILURE;
        }

        size_t input_len = strnlen(inputs[i], MAX_STRING_LENGTH);
        if (input_len >= MAX_STRING_LENGTH) {
            for (size_t j = 0; j < i; j++) {
                free(strings[j]);
                strings[j] = NULL;
            }
            return EXIT_FAILURE;
        }

        strings[i] = (char *)malloc(MAX_STRING_LENGTH);
        if (strings[i] == NULL) {
            for (size_t j = 0; j < i; j++) {
                free(strings[j]);
                strings[j] = NULL;
            }
            return EXIT_FAILURE;
        }

        int written = snprintf(strings[i], MAX_STRING_LENGTH, "%s", inputs[i]);
        if (written < 0 || (size_t)written >= MAX_STRING_LENGTH) {
            for (size_t j = 0; j <= i; j++) {
                free(strings[j]);
                strings[j] = NULL;
            }
            return EXIT_FAILURE;
        }
    }

    printf("Before:\n");
    for (size_t i = 0; i < size && i < MAX_INPUTS; i++) {
        if (strings[i] != NULL) {
            printf("%s\n", strings[i]);
        }
    }

    remove_digits(strings, size, MAX_STRING_LENGTH);

    printf("\nAfter:\n");
    for (size_t i = 0; i < size && i < MAX_INPUTS; i++) {
        if (strings[i] != NULL) {
            printf("%s\n", strings[i]);
        }
    }

    for (size_t i = 0; i < size && i < MAX_INPUTS; i++) {
        free(strings[i]);
        strings[i] = NULL;
    }

    return EXIT_SUCCESS;
}