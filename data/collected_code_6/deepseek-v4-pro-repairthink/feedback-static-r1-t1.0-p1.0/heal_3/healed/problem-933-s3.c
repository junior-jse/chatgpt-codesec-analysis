#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char *camel_to_snake(const char *input, size_t input_len) {
    if (input == NULL) return NULL;
    size_t extra_underscores = 0;
    for (size_t i = 1; i < input_len; i++) {
        if (isupper((unsigned char)input[i])) {
            extra_underscores++;
        }
    }
    size_t new_len = input_len + extra_underscores + 1;
    char *result = malloc(new_len);
    if (result == NULL) return NULL;
    size_t j = 0;
    for (size_t i = 0; i < input_len; i++) {
        unsigned char c = (unsigned char)input[i];
        if (isupper(c)) {
            if (i > 0) {
                result[j++] = '_';
            }
            result[j++] = (char)tolower(c);
        } else {
            result[j++] = (char)c;
        }
    }
    result[j] = '\0';
    return result;
}

#define MAX_STRING_LENGTH 1024

int main(void) {
    const char *test_strings[] = {
        "camelCaseString",
        "thisIsATest",
        "single",
        "",
        "ALLCAPS",
        "mixedCASEString"
    };
    size_t num_tests = sizeof(test_strings) / sizeof(test_strings[0]);
    for (size_t i = 0; i < num_tests; i++) {
        size_t len = strnlen(test_strings[i], MAX_STRING_LENGTH);
        char *converted = camel_to_snake(test_strings[i], len);
        if (converted != NULL) {
            printf("%s -> %s\n", test_strings[i], converted);
            free(converted);
        }
    }
    return 0;
}