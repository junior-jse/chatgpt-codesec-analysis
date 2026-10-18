#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char *camel_to_snake(const char *input) {
    if (input == NULL) {
        return NULL;
    }

    size_t cap = 16;
    char *result = malloc(cap);
    if (result == NULL) {
        return NULL;
    }

    size_t j = 0;
    for (const char *p = input; *p != '\0'; p++) {
        unsigned char c = (unsigned char)*p;

        if (isupper(c)) {
            if (j > 0 && result[j - 1] != '_') {
                if (j + 2 > cap) {
                    cap *= 2;
                    char *tmp = realloc(result, cap);
                    if (tmp == NULL) {
                        free(result);
                        return NULL;
                    }
                    result = tmp;
                }
                result[j++] = '_';
            }

            if (j + 1 > cap) {
                cap *= 2;
                char *tmp = realloc(result, cap);
                if (tmp == NULL) {
                    free(result);
                    return NULL;
                }
                result = tmp;
            }
            result[j++] = (char)tolower(c);
        } else {
            if (j + 1 > cap) {
                cap *= 2;
                char *tmp = realloc(result, cap);
                if (tmp == NULL) {
                    free(result);
                    return NULL;
                }
                result = tmp;
            }
            result[j++] = (char)c;
        }
    }

    if (j + 1 > cap) {
        char *tmp = realloc(result, j + 1);
        if (tmp == NULL) {
            free(result);
            return NULL;
        }
        result = tmp;
    }
    result[j] = '\0';

    return result;
}

int main(void) {
    const char *tests[] = {
        "camelCase",
        "PascalCase",
        "already_snake",
        "ABC",
        "aBc",
        "",
        "helloWorldExample",
        "CamelCaseTest123",
        NULL
    };

    for (int i = 0; tests[i] != NULL; i++) {
        char *converted = camel_to_snake(tests[i]);
        if (converted != NULL) {
            printf("%s -> %s\n", tests[i], converted);
            free(converted);
        } else {
            printf("%s -> (null)\n", tests[i]);
        }
    }

    return 0;
}