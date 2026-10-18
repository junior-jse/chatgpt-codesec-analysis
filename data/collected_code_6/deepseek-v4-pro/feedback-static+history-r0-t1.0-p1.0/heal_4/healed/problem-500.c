#include <stdlib.h>
#include <string.h>

char *concatenate_list(const char *const list[], size_t count) {
    size_t total_length = 0;
    for (size_t i = 0; i < count; i++) {
        if (list[i] != NULL) {
            size_t len = strnlen(list[i], SIZE_MAX);
            total_length += len;
            if (total_length < len) {
                return NULL;
            }
        }
    }

    char *result = (char *)malloc(total_length + 1);
    if (result == NULL) {
        return NULL;
    }

    size_t offset = 0;
    for (size_t i = 0; i < count; i++) {
        if (list[i] != NULL) {
            size_t remaining = total_length - offset;
            size_t len = strnlen(list[i], remaining);
            if (len > remaining) {
                free(result);
                return NULL;
            }
            memcpy(result + offset, list[i], len);
            offset += len;
        }
    }
    result[offset] = '\0';

    return result;
}

int main(void) {
    const char *words[] = {"Hello", " ", "World", "!"};
    size_t count = sizeof(words) / sizeof(words[0]);

    char *result = concatenate_list(words, count);
    if (result != NULL) {
        free(result);
    }

    return 0;
}