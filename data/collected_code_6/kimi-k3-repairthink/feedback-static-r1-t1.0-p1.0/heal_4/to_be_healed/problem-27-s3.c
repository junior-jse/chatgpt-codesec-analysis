#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_STRING_LENGTH 1024
#define MAX_INPUTS 16

void remove_digits(char **list, size_t size) {
    if (list == NULL || size == 0) {
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
        for (size_t read = 0; read < MAX_STRING_LENGTH - 1 && list[i][read] != '\0'; read++) {
            /* Possible weaknesses found:
             * Flawfinder read: Check buffer boundaries if used in a loop including recursive loops (CWE-120, CWE-20). (risk 1, buffer)
             */
            unsigned char ch = (unsigned char)list[i][read];

            if (!isdigit(ch)) {
                if (write >= MAX_STRING_LENGTH - 1) {
                    break;
                }
                list[i][write] = (char)ch;
                write++;
            }
        }

        list[i][write] = '\0';
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
    char *strings[MAX_INPUTS] = { NULL };

    for (size_t i = 0; i < size; i++) {
        strings[i] = malloc(MAX_STRING_LENGTH);
        if (strings[i] == NULL) {
            for (size_t j = 0; j < i; j++) {
                free(strings[j]);
            }
            return EXIT_FAILURE;
        }

        int written = snprintf(strings[i], MAX_STRING_LENGTH, "%s", inputs[i]);
        if (written < 0 || (size_t)written >= MAX_STRING_LENGTH) {
            strings[i][0] = '\0';
        }
    }

    printf("Before:\n");
    for (size_t i = 0; i < size; i++) {
        printf("%s\n", strings[i]);
    }

    remove_digits(strings, size);

    printf("\nAfter:\n");
    for (size_t i = 0; i < size; i++) {
        printf("%s\n", strings[i]);
    }

    for (size_t i = 0; i < size; i++) {
        free(strings[i]);
    }

    return EXIT_SUCCESS;
}