#include <stdlib.h>
#include <string.h>

char *concatenate_list(const char *const list[], size_t count) {
    size_t total_length = 0;
    for (size_t i = 0; i < count; i++) {
        if (list[i] != NULL) {
            total_length += strlen(list[i]);
        }
    }

    char *result = (char *)malloc(total_length + 1);
    if (result == NULL) {
        return NULL;
    }

    result[0] = '\0';
    for (size_t i = 0; i < count; i++) {
        if (list[i] != NULL) {
            strcat(result, list[i]);
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