#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <regex.h>

int replace_chars_with_colon(char *str, size_t buffer_size) {
    regex_t regex;
    regmatch_t match;
    size_t str_len;
    size_t pos = 0;
    int ret;

    if (str == NULL || buffer_size == 0) {
        return -1;
    }

    str_len = strnlen(str, buffer_size);
    if (str_len >= buffer_size) {
        return -1;
    }

    ret = regcomp(&regex, "[ ,.]", REG_EXTENDED);
    if (ret != 0) {
        return -1;
    }

    while (pos < str_len) {
        ret = regexec(&regex, str + pos, 1, &match, 0);
        if (ret == REG_NOMATCH) {
            break;
        }
        if (ret != 0) {
            regfree(&regex);
            return -1;
        }

        if (match.rm_so < 0 || match.rm_eo <= match.rm_so ||
            (size_t)match.rm_eo > str_len - pos) {
            regfree(&regex);
            return -1;
        }

        str[pos + (size_t)match.rm_so] = ':';
        pos += (size_t)match.rm_eo;
    }

    regfree(&regex);
    return 0;
}

int main(void) {
    /* Possible weaknesses found:
     *  Assignment 'source="Hello, World. This is a test string, with spaces."', assigned value is "Hello, World. This is a test string, with spaces."
     */
    const char *source = "Hello, World. This is a test string, with spaces.";
    size_t source_len;
    size_t buffer_size;
    char *test_string;
    int status;

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     *  Assignment 'source_len=strlen(source)', assigned value is 49
     */
    source_len = strlen(source);

    /* Possible weaknesses found:
     *  Condition 'source_len==0' is always false [knownConditionTrueFalse]
     *  Condition 'source_len==0' is always false
     */
    if (source_len == 0 || source_len >= SIZE_MAX) {
        fprintf(stderr, "Error: Invalid source string length\n");
        return EXIT_FAILURE;
    }

    buffer_size = source_len + 1;

    test_string = malloc(buffer_size);
    if (test_string == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(test_string, source, buffer_size);

    printf("Original string: %s\n", test_string);

    status = replace_chars_with_colon(test_string, buffer_size);
    if (status != 0) {
        fprintf(stderr, "Error: Failed to replace characters\n");
        free(test_string);
        return EXIT_FAILURE;
    }

    printf("Modified string: %s\n", test_string);

    free(test_string);
    return EXIT_SUCCESS;
}