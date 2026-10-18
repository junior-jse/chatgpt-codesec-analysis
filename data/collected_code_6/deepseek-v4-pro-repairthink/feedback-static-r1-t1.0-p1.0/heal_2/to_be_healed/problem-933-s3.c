#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char *camel_to_snake(const char *input) {
    if (input == NULL) return NULL;
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t len = strlen(input);
    size_t extra_underscores = 0;
    for (size_t i = 1; i < len; i++) {
        if (isupper((unsigned char)input[i])) {
            extra_underscores++;
        }
    }
    size_t new_len = len + extra_underscores + 1;
    char *result = malloc(new_len);
    if (result == NULL) return NULL;
    size_t j = 0;
    for (size_t i = 0; i < len; i++) {
        unsigned char c = input[i];
        if (isupper(c)) {
            if (i > 0) {
                result[j++] = '_';
            }
            result[j++] = (char)tolower(c);
        } else {
            result[j++] = c;
        }
    }
    result[j] = '\0';
    return result;
}

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
        char *converted = camel_to_snake(test_strings[i]);
        if (converted != NULL) {
            printf("%s -> %s\n", test_strings[i], converted);
            free(converted);
        }
    }
    return 0;
}