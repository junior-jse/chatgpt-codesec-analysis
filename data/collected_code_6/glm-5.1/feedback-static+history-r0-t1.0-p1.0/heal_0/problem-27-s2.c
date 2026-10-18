#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

void remove_digits_from_string(char *str) {
    if (str == NULL) {
        return;
    }
    char *dst = str;
    char *src = str;
    while (*src != '\0') {
        if (!isdigit((unsigned char)*src)) {
            *dst = *src;
            dst++;
        }
        src++;
    }
    *dst = '\0';
}

void remove_digits_from_list(char **list, size_t count) {
    if (list == NULL) {
        return;
    }
    for (size_t i = 0; i < count; i++) {
        remove_digits_from_string(list[i]);
    }
}

int main(void) {
    char *list[] = {
        "hello123",
        "w0rld",
        "425c0de",
        "nodigits"
    };
    size_t count = sizeof(list) / sizeof(list[0]);

    for (size_t i = 0; i < count; i++) {
        char *mutable = strdup(list[i]);
        if (mutable == NULL) {
            return EXIT_FAILURE;
        }
        remove_digits_from_string(mutable);
        printf("%s\n", mutable);
        free(mutable);
    }

    return EXIT_SUCCESS;
}