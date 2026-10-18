#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char *camel_to_snake(const char *input) {
    if (input == NULL) {
        return NULL;
    }
    size_t len = strlen(input);
    size_t underscores = 0;
    for (size_t i = 1; i < len; i++) {
        if ((islower((unsigned char)input[i-1]) || isdigit((unsigned char)input[i-1])) && isupper((unsigned char)input[i])) {
            underscores++;
        }
    }
    size_t out_len = len + underscores + 1;
    char *result = (char *)malloc(out_len);
    if (result == NULL) {
        return NULL;
    }
    size_t dst = 0;
    for (size_t i = 0; i < len; i++) {
        if (i > 0 && (islower((unsigned char)input[i-1]) || isdigit((unsigned char)input[i-1])) && isupper((unsigned char)input[i])) {
            result[dst++] = '_';
            result[dst++] = (char)tolower((unsigned char)input[i]);
        } else {
            result[dst++] = input[i];
        }
    }
    result[dst] = '\0';
    return result;
}

int main(void) {
    const char *test_strings[] = {
        "camelCase",
        "snake_case",
        "myVariableName",
        "HTTPResponseCode",
        "test123Case",
        "already_snake",
        "A",
        "",
        "ABC",
        "convertJSONToXML"
    };
    size_t test_count = sizeof(test_strings) / sizeof(test_strings[0]);
    for (size_t i = 0; i < test_count; i++) {
        char *converted = camel_to_snake(test_strings[i]);
        if (converted != NULL) {
            printf("\"%s\" -> \"%s\"\n", test_strings[i], converted);
            free(converted);
        } else {
            printf("\"%s\" -> conversion failed\n", test_strings[i]);
        }
    }
    return 0;
}