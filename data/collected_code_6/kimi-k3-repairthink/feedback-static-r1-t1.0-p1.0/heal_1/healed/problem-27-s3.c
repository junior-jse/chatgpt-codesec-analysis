#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_STRING_LENGTH 1024

void remove_digits(char **list, size_t size) {
    if (list == NULL || size == 0) {
        return;
    }

    for (size_t i = 0; i < size; i++) {
        if (list[i] == NULL) {
            continue;
        }

        size_t length = strnlen(list[i], MAX_STRING_LENGTH);
        size_t write = 0;

        for (size_t read = 0; read < length; read++) {
            if (!isdigit((unsigned char)list[i][read])) {
                list[i][write] = list[i][read];
                write++;
            }
        }

        if (write < MAX_STRING_LENGTH) {
            list[i][write] = '\0';
        } else {
            list[i][MAX_STRING_LENGTH - 1] = '\0';
        }
    }
}

int main(void) {
    char *strings[] = {
        strdup("hello123world"),
        strdup("abc456def"),
        strdup("no_digits_here"),
        strdup("12345")
    };
    size_t size = sizeof(strings) / sizeof(strings[0]);

    for (size_t i = 0; i < size; i++) {
        if (strings[i] == NULL) {
            for (size_t j = 0; j < i; j++) {
                free(strings[j]);
            }
            return EXIT_FAILURE;
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