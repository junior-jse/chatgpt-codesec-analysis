#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char *remove_non_alphanumeric(const char *input) {
    if (input == NULL) {
        return NULL;
    }

    size_t len = 0;
    const char *p;
    for (p = input; *p != '\0'; p++) {
        if (isalnum((unsigned char)*p)) {
            len++;
        }
    }

    char *result = malloc(len + 1);
    if (result == NULL) {
        return NULL;
    }

    size_t j = 0;
    for (p = input; *p != '\0'; p++) {
        if (isalnum((unsigned char)*p)) {
            result[j++] = *p;
        }
    }
    result[j] = '\0';

    return result;
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