#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *concat_adjacent_elements(const char *tuple[], int size) {
    if (tuple == NULL || size <= 0) {
        return NULL;
    }

    size_t total_length = 0;
    for (int i = 0; i < size; i++) {
        if (tuple[i] == NULL) {
            return NULL;
        }
        size_t len = strlen(tuple[i]);
        if (total_length > SIZE_MAX - len) {
            return NULL;
        }
        total_length += len;
    }

    if (total_length == SIZE_MAX) {
        return NULL;
    }

    char *result = (char *)malloc(total_length + 1);
    if (result == NULL) {
        return NULL;
    }

    size_t pos = 0;
    for (int i = 0; i < size; i++) {
        size_t len = strlen(tuple[i]);
        if (pos + len > total_length) {
            free(result);
            return NULL;
        }
        memcpy(result + pos, tuple[i], len);
        pos += len;
    }
    result[pos] = '\0';

    return result;
}

int main(void) {
    const char *tuple1[] = {"hello", "world", "123"};
    int size1 = sizeof(tuple1) / sizeof(tuple1[0]);
    
    char *result1 = concat_adjacent_elements(tuple1, size1);
    if (result1 != NULL) {
        printf("%s\n", result1);
        free(result1);
    }

    const char *tuple2[] = {"a", "b", "c", "d"};
    int size2 = sizeof(tuple2) / sizeof(tuple2[0]);
    
    char *result2 = concat_adjacent_elements(tuple2, size2);
    if (result2 != NULL) {
        printf("%s\n", result2);
        free(result2);
    }

    const char *tuple3[] = {"single"};
    int size3 = sizeof(tuple3) / sizeof(tuple3[0]);
    
    char *result3 = concat_adjacent_elements(tuple3, size3);
    if (result3 != NULL) {
        printf("%s\n", result3);
        free(result3);
    }

    return 0;
}