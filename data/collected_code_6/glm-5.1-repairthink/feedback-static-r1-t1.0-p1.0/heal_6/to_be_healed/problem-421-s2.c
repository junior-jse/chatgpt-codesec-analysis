#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static size_t safe_strnlen(const char* s, size_t maxlen) {
    const char* p = memchr(s, '\0', maxlen);
    return p ? (size_t)(p - s) : maxlen;
}

char* concatenate_tuple(const char* elements[], size_t count, const char* delimiter) {
    if (count == 0 || elements == NULL || delimiter == NULL) {
        char* result = malloc(1);
        if (result != NULL) {
            result[0] = '\0';
        }
        return result;
    }

    size_t total_length = 0;
    size_t delimiter_length = safe_strnlen(delimiter, SIZE_MAX);
    int first = 1;

    for (size_t i = 0; i < count; i++) {
        if (elements[i] != NULL) {
            if (!first) {
                total_length += delimiter_length;
            }
            total_length += safe_strnlen(elements[i], SIZE_MAX);
            first = 0;
        }
    }

    char* result = malloc(total_length + 1);
    if (result == NULL) {
        return NULL;
    }

    char* ptr = result;
    /* Possible weaknesses found:
     *  Variable 'end' can be declared as pointer to const [constVariablePointer]
     */
    char* end = result + total_length;
    first = 1;

    for (size_t i = 0; i < count; i++) {
        if (elements[i] != NULL) {
            if (!first) {
                size_t copy_len = delimiter_length;
                if (ptr + copy_len > end) {
                    copy_len = end - ptr;
                }
                if (copy_len > 0) {
                    /* Possible weaknesses found:
                     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
                     */
                    memcpy(ptr, delimiter, copy_len);
                    ptr += copy_len;
                }
            }
            
            size_t len = safe_strnlen(elements[i], SIZE_MAX);
            size_t copy_len = len;
            if (ptr + copy_len > end) {
                copy_len = end - ptr;
            }
            if (copy_len > 0) {
                /* Possible weaknesses found:
                 * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
                 */
                memcpy(ptr, elements[i], copy_len);
                ptr += copy_len;
            }
            first = 0;
        }
    }

    *ptr = '\0';

    return result;
}

int main(void) {
    const char* tuple[] = {"apple", "banana", "cherry"};
    size_t tuple_size = sizeof(tuple) / sizeof(tuple[0]);
    const char* delimiter = ", ";

    char* concatenated = concatenate_tuple(tuple, tuple_size, delimiter);
    
    if (concatenated != NULL) {
        printf("%s\n", concatenated);
        free(concatenated);
    }

    return 0;
}