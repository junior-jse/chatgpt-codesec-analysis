#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

char *camel_to_snake(const char *input) {
    if (input == NULL) return NULL;

    size_t len = 0;
    int prev = 0;
    for (const char *s = input; *s; s++) {
        if (prev && (islower((unsigned char)prev) || isdigit((unsigned char)prev))
            && isupper((unsigned char)*s))
            len++;
        len++;
        prev = *s;
    }

    char *output = malloc(len + 1);
    if (output == NULL) return NULL;

    char *out = output;
    prev = 0;
    for (const char *s = input; *s; s++) {
        if (prev && (islower((unsigned char)prev) || isdigit((unsigned char)prev))
            && isupper((unsigned char)*s)) {
            *out++ = '_';
            *out++ = tolower((unsigned char)*s);
        } else {
            *out++ = *s;
        }
        prev = *s;
    }
    *out = '\0';

    return output;
}

int main(void) {
    const char *tests[] = {
        "camelCase",
        "helloWorld",
        "thisIsATest",
        "already_snake",
        "mixedCase_WithSnake",
        "UPPERCASE",
        "camel123Case",
        "c"
    };
    size_t num_tests = sizeof(tests) / sizeof(tests[0]);

    for (size_t i = 0; i < num_tests; i++) {
        char *result = camel_to_snake(tests[i]);
        if (result != NULL) {
            printf("%s -> %s\n", tests[i], result);
            free(result);
        } else {
            printf("%s -> (conversion failed)\n", tests[i]);
        }
    }

    return 0;
}