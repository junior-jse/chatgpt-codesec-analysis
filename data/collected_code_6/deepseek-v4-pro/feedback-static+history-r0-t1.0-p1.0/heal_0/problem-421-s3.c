#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

char* concat_elements(const char* delimiter, int count, ...) {
    if (delimiter == NULL || count <= 0) {
        return NULL;
    }

    va_list args;
    va_start(args, count);

    size_t total_len = 0;
    const char** elements = (const char**)malloc(count * sizeof(const char*));
    if (elements == NULL) {
        va_end(args);
        return NULL;
    }

    for (int i = 0; i < count; i++) {
        elements[i] = va_arg(args, const char*);
        if (elements[i] == NULL) {
            free(elements);
            va_end(args);
            return NULL;
        }
        total_len += strlen(elements[i]);
        if (i < count - 1) {
            total_len += strlen(delimiter);
        }
    }

    char* result = (char*)malloc(total_len + 1);
    if (result == NULL) {
        free(elements);
        va_end(args);
        return NULL;
    }

    result[0] = '\0';
    for (int i = 0; i < count; i++) {
        strcat(result, elements[i]);
        if (i < count - 1) {
            strcat(result, delimiter);
        }
    }

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