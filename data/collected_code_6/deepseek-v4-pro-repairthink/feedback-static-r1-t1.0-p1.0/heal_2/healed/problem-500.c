#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_STR_LEN 4096

char *concatenate_list(const char *const list[], size_t count) {
    size_t total_length = 0;
    for (size_t i = 0; i < count; i++) {
        if (list[i] != NULL) {
            total_length += strnlen(list[i], MAX_STR_LEN);
        }
    }

    char *result = (char *)malloc(total_length + 1);
    if (result == NULL) {
        return NULL;
    }

    char *dest = result;
    for (size_t i = 0; i < count; i++) {
        if (list[i] != NULL) {
            int written = snprintf(dest, total_length + 1 - (size_t)(dest - result), "%s", list[i]);
            if (written < 0) {
                free(result);
                return NULL;
            }
            dest += written;
        }
    }

    return result;
}

int main(void) {
    const char *words[] = {"Hello", " ", "World", "!", NULL};
    size_t count = sizeof(words) / sizeof(words[0]) - 1;

    char *result = concatenate_list(words, count);
    if (result != NULL) {
        free(result);
    }

    return 0;
}