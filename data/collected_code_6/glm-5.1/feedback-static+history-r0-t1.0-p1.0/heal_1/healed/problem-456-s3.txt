#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void reverse_strings(char **strings, size_t count) {
    for (size_t i = 0; i < count; i++) {
        if (strings[i] != NULL) {
            size_t len = strnlen(strings[i], SIZE_MAX);
            if (len > 0) {
                for (size_t left = 0, right = len - 1; left < right; left++, right--) {
                    char temp = strings[i][left];
                    strings[i][left] = strings[i][right];
                    strings[i][right] = temp;
                }
            }
        }
    }
}

int main(void) {
    size_t count = 5;
    char **list = malloc(count * sizeof(char *));
    if (list == NULL) {
        return EXIT_FAILURE;
    }

    list[0] = malloc(6 * sizeof(char));
    list[1] = malloc(6 * sizeof(char));
    list[2] = malloc(2 * sizeof(char));
    list[3] = malloc(5 * sizeof(char));
    list[4] = NULL;

    if (!list[0] || !list[1] || !list[2] || !list[3]) {
        for (size_t i = 0; i < 4; i++) {
            free(list[i]);
        }
        free(list);
        return EXIT_FAILURE;
    }

    snprintf(list[0], 6, "%s", "hello");
    snprintf(list[1], 6, "%s", "world");
    snprintf(list[2], 2, "%s", "");
    snprintf(list[3], 5, "%s", "test");

    reverse_strings(list, count);

    for (size_t i = 0; i < count; i++) {
        if (list[i] != NULL) {
            printf("%s\n", list[i]);
        }
    }

    for (size_t i = 0; i < 4; i++) {
        free(list[i]);
    }
    free(list);

    return EXIT_SUCCESS;
}