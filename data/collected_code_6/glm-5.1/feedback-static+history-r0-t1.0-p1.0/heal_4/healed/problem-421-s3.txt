#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* tuple_concat(const char** tuple, size_t size, const char* delimiter) {
    if (tuple == NULL || delimiter == NULL || size == 0) {
        return NULL;
    }

    size_t total_len = 0;
    size_t delim_len = strlen(delimiter);

    for (size_t i = 0; i < size; i++) {
        if (tuple[i] == NULL) {
            return NULL;
        }
        total_len += strlen(tuple[i]);
        if (i < size - 1) {
            total_len += delim_len;
        }
    }

    char* result = (char*)malloc(total_len + 1);
    if (result == NULL) {
        return NULL;
    }

    char* ptr = result;
    size_t remaining = total_len + 1;

    for (size_t i = 0; i < size; i++) {
        size_t item_len = strlen(tuple[i]);
        if (item_len > remaining - 1) {
            item_len = remaining - 1;
        }
        memcpy(ptr, tuple[i], item_len);
        ptr += item_len;
        remaining -= item_len;
        
        if (i < size - 1) {
            size_t copy_delim_len = delim_len;
            if (copy_delim_len > remaining - 1) {
                copy_delim_len = remaining - 1;
            }
            memcpy(ptr, delimiter, copy_delim_len);
            ptr += copy_delim_len;
            remaining -= copy_delim_len;
        }
    }

    if (remaining > 0) {
        *ptr = '\0';
    }

    return result;
}

int main(void) {
    const char* tuple[] = {"C", "is", "a", "powerful", "language"};
    size_t size = sizeof(tuple) / sizeof(tuple[0]);
    const char* delimiter = "-";

    char* concatenated = tuple_concat(tuple, size, delimiter);
    if (concatenated != NULL) {
        printf("%s\n", concatenated);
        free(concatenated);
        concatenated = NULL;
    }

    return 0;
}