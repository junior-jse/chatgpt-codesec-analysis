#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#define MAX_STRING_LENGTH 1024

int replace_chars_with_colon(char *str, size_t buffer_size) {
    regex_t regex;
    regmatch_t match;
    /* Possible weaknesses found:
     *  Unused variable: temp [unusedVariable]
     */
    char *temp;
    char *result;
    size_t str_len;
    size_t result_len = 0;
    size_t pos = 0;
    int ret;

    if (str == NULL || buffer_size == 0) {
        return -1;
    }

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    str_len = strlen(str);
    if (str_len >= buffer_size) {
        return -1;
    }

    ret = regcomp(&regex, "[ ,.]", REG_EXTENDED);
    if (ret != 0) {
        return -1;
    }

    result = (char *)malloc(buffer_size);
    if (result == NULL) {
        regfree(&regex);
        return -1;
    }

    result[0] = '\0';

    while (pos < str_len) {
        ret = regexec(&regex, str + pos, 1, &match, 0);
        if (ret == REG_NOMATCH) {
            break;
        } else if (ret != 0) {
            free(result);
            regfree(&regex);
            return -1;
        }

        size_t copy_len = (size_t)match.rm_so;
        if (result_len + copy_len + 2 > buffer_size) {
            free(result);
            regfree(&regex);
            return -1;
        }

        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(result + result_len, str + pos, copy_len);
        result_len += copy_len;
        result[result_len++] = ':';
        result[result_len] = '\0';

        pos += (size_t)match.rm_eo;
    }

    if (result_len + (str_len - pos) + 1 > buffer_size) {
        free(result);
        regfree(&regex);
        return -1;
    }

    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(result + result_len, str + pos, str_len - pos);
    result_len += str_len - pos;
    result[result_len] = '\0';

    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(str, result, result_len + 1);

    free(result);
    regfree(&regex);

    return 0;
}

int main(void) {
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char test_string[MAX_STRING_LENGTH] = "Hello, World. This is a test string, with spaces.";
    int status;

    printf("Original string: %s\n", test_string);

    status = replace_chars_with_colon(test_string, sizeof(test_string));
    if (status != 0) {
        fprintf(stderr, "Error: Failed to replace characters\n");
        return EXIT_FAILURE;
    }

    printf("Modified string: %s\n", test_string);

    return EXIT_SUCCESS;
}