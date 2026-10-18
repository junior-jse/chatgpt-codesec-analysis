#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char *remove_non_alphanumeric(const char *input) {
    if (input == NULL) {
        return NULL;
    }

    size_t input_len = strnlen(input, SIZE_MAX);
    
    char *result = (char *)malloc(input_len + 1);
    if (result == NULL) {
        return NULL;
    }
    
    size_t result_len = 0;
    const char *cursor = input;
    
    while (*cursor != '\0') {
        if ((*cursor >= 'a' && *cursor <= 'z') ||
            (*cursor >= 'A' && *cursor <= 'Z') ||
            (*cursor >= '0' && *cursor <= '9')) {
            if (result_len < input_len) {
                result[result_len++] = *cursor;
            }
        }
        cursor++;
    }
    
    result[result_len] = '\0';
    
    char *final_result = (char *)realloc(result, result_len + 1);
    if (final_result == NULL) {
        free(result);
        return NULL;
    }
    
    return final_result;
}

int main(void) {
    const char *test_strings[] = {
        "Hello, World! 123",
        "Test@#$%^&*()String",
        "abc123XYZ",
        "   spaces   and---symbols!!!",
        "",
        NULL
    };

    for (int i = 0; test_strings[i] != NULL; i++) {
        char *cleaned = remove_non_alphanumeric(test_strings[i]);
        if (cleaned != NULL) {
            printf("Original: %s\n", test_strings[i]);
            printf("Cleaned:  %s\n\n", cleaned);
            free(cleaned);
        } else {
            printf("Failed to process string: %s\n\n", test_strings[i]);
        }
    }

    return 0;
}