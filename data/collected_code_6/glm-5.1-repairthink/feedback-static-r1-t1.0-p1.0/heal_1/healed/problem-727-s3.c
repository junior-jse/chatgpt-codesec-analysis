#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char *remove_non_alnum(const char *input, size_t input_size) {
    if (input == NULL) {
        return NULL;
    }

    size_t input_len = strnlen(input, input_size);
    char *result = malloc(input_len + 1);
    if (result == NULL) {
        return NULL;
    }

    size_t result_idx = 0;
    for (size_t i = 0; i < input_len; i++) {
        if (isalnum((unsigned char)input[i])) {
            result[result_idx++] = input[i];
        }
    }

    result[result_idx] = '\0';

    char *shrunk = realloc(result, result_idx + 1);
    if (shrunk != NULL) {
        result = shrunk;
    }

    return result;
}

int main(void) {
    const char *str1 = "H3ll0, W@rld! 123.";
    char *cleaned = remove_non_alnum(str1, strlen(str1) + 1);
    if (cleaned != NULL) {
        printf("%s\n", cleaned);
        free(cleaned);
    }

    const char *str2 = "___---((()))___";
    cleaned = remove_non_alnum(str2, strlen(str2) + 1);
    if (cleaned != NULL) {
        printf("%s\n", cleaned);
        free(cleaned);
    }

    const char *str3 = "NoSpecialChars123";
    cleaned = remove_non_alnum(str3, strlen(str3) + 1);
    if (cleaned != NULL) {
        printf("%s\n", cleaned);
        free(cleaned);
    }

    return 0;
}