#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdint.h>

char* concat_elements(const char* delimiter, int count, ...) {
    if (delimiter == NULL || count <= 0) {
        return NULL;
    }

    size_t delim_len = strnlen(delimiter, SIZE_MAX);
    
    va_list args;
    va_start(args, count);

    const char** elements = (const char**)malloc(count * sizeof(const char*));
    if (elements == NULL) {
        va_end(args);
        return NULL;
    }

    size_t total_len = 0;
    for (int i = 0; i < count; i++) {
        elements[i] = va_arg(args, const char*);
        if (elements[i] == NULL) {
            free(elements);
            va_end(args);
            return NULL;
        }
        
        size_t elem_len = strnlen(elements[i], SIZE_MAX);
        if (total_len > SIZE_MAX - elem_len) {
            free(elements);
            va_end(args);
            return NULL;
        }
        total_len += elem_len;
        
        if (i < count - 1) {
            if (total_len > SIZE_MAX - delim_len) {
                free(elements);
                va_end(args);
                return NULL;
            }
            total_len += delim_len;
        }
    }

    if (total_len >= SIZE_MAX) {
        free(elements);
        va_end(args);
        return NULL;
    }

    char* result = (char*)malloc(total_len + 1);
    if (result == NULL) {
        free(elements);
        va_end(args);
        return NULL;
    }

    size_t offset = 0;
    for (int i = 0; i < count; i++) {
        size_t elem_len = strnlen(elements[i], SIZE_MAX);
        if (offset + elem_len > total_len) {
            free(result);
            free(elements);
            va_end(args);
            return NULL;
        }
        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(result + offset, elements[i], elem_len);
        offset += elem_len;
        
        if (i < count - 1) {
            if (offset + delim_len > total_len) {
                free(result);
                free(elements);
                va_end(args);
                return NULL;
            }
            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
            memcpy(result + offset, delimiter, delim_len);
            offset += delim_len;
        }
    }
    result[offset] = '\0';

    free(elements);
    va_end(args);
    return result;
}

int main(void) {
    char* result1 = concat_elements("-", 3, "a", "b", "c");
    if (result1 != NULL) {
        printf("%s\n", result1);
        free(result1);
    }

    char* result2 = concat_elements(", ", 4, "one", "two", "three", "four");
    if (result2 != NULL) {
        printf("%s\n", result2);
        free(result2);
    }

    char* result3 = concat_elements("", 3, "x", "y", "z");
    if (result3 != NULL) {
        printf("%s\n", result3);
        free(result3);
    }

    char* result4 = concat_elements("-", 1, "single");
    if (result4 != NULL) {
        printf("%s\n", result4);
        free(result4);
    }

    char* result5 = concat_elements(NULL, 3, "a", "b", "c");
    if (result5 != NULL) {
        printf("%s\n", result5);
        free(result5);
    }

    char* result6 = concat_elements("-", 0);
    if (result6 != NULL) {
        printf("%s\n", result6);
        free(result6);
    }

    return 0;
}