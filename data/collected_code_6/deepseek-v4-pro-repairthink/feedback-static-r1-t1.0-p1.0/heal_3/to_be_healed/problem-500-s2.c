#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRLEN 4096

static size_t safe_strnlen(const char *s, size_t maxlen) {
    size_t i = 0;
    while (i < maxlen && s[i] != '\0') {
        i++;
    }
    return i;
}

char *concatenate_list(char **list, int count) {
    size_t total_length = 0;
    size_t *lengths;
    char *result;
    char *position;
    int i;

    if (list == NULL || count <= 0) {
        return NULL;
    }

    lengths = malloc((size_t)count * sizeof(size_t));
    if (lengths == NULL) {
        return NULL;
    }

    for (i = 0; i < count; i++) {
        if (list[i] != NULL) {
            size_t len = safe_strnlen(list[i], MAX_STRLEN);
            if (len == MAX_STRLEN) {
                free(lengths);
                return NULL;
            }
            if (total_length > SIZE_MAX - len) {
                free(lengths);
                return NULL;
            }
            lengths[i] = len;
            total_length += len;
        } else {
            lengths[i] = 0;
        }
    }

    result = malloc(total_length + 1);
    if (result == NULL) {
        free(lengths);
        return NULL;
    }

    position = result;
    for (i = 0; i < count; i++) {
        if (list[i] != NULL) {
            size_t len = lengths[i];
            if (position + len > result + total_length) {
                free(lengths);
                free(result);
                return NULL;
            }
            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
            memcpy(position, list[i], len);
            position += len;
        }
    }
    *position = '\0';

    free(lengths);
    return result;
}

int main(void) {
    char *list1[] = {"Hello", " ", "World", "!"};
    char *list2[] = {"The", " ", "quick", " ", "brown", " ", "fox"};
    char *list3[] = {"", "Empty", "", "String", ""};
    char *result;

    result = concatenate_list(list1, 4);
    if (result != NULL) {
        printf("%s\n", result);
        free(result);
    }

    result = concatenate_list(list2, 7);
    if (result != NULL) {
        printf("%s\n", result);
        free(result);
    }

    result = concatenate_list(list3, 5);
    if (result != NULL) {
        printf("[%s]\n", result);
        free(result);
    }

    result = concatenate_list(NULL, 3);
    if (result == NULL) {
        printf("NULL input handled\n");
    }

    return 0;
}