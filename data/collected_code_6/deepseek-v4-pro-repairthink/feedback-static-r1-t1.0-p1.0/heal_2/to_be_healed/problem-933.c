#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

char *camel_to_snake(const char *input) {
    regex_t regex;
    regmatch_t matches[3];
    const char *pattern = "([a-z0-9])([A-Z])";
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t input_len = strlen(input);
    size_t output_len = input_len + 1;
    const char *cursor;
    char *output;
    char *out_ptr;

    if (regcomp(&regex, pattern, REG_EXTENDED) != 0) {
        return NULL;
    }

    cursor = input;
    while (regexec(&regex, cursor, 3, matches, 0) == 0) {
        output_len++;
        cursor += matches[0].rm_eo;
    }

    output = malloc(output_len);
    if (output == NULL) {
        regfree(&regex);
        return NULL;
    }

    cursor = input;
    out_ptr = output;

    while (regexec(&regex, cursor, 3, matches, 0) == 0) {
        size_t prefix_len = matches[1].rm_eo - matches[1].rm_so;
        size_t match_len = matches[0].rm_eo - matches[0].rm_so;

        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(out_ptr, cursor, prefix_len);
        out_ptr += prefix_len;

        *out_ptr++ = '_';
        *out_ptr++ = cursor[matches[2].rm_so] + ('a' - 'A');

        cursor += match_len;
    }

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t remaining_len = strlen(cursor);
    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(out_ptr, cursor, remaining_len + 1);

    regfree(&regex);
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