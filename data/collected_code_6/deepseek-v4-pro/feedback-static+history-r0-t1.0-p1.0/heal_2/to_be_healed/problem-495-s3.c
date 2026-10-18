/* Repair history (previous round):
 * - Flawfinder strcpy was replaced by memcpy, which Flawfinder flags with the same CWE (CWE-120).
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

char *remove_lowercase_substrings(const char *input) {
    regex_t regex;
    regmatch_t match;
    const char *pattern = "[a-z]+";
    size_t input_len;
    char *result;
    size_t result_len = 0;
    const char *cursor;
    int status;
    size_t remaining_len;

    if (!input) {
        return NULL;
    }

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    input_len = strlen(input);
    result = malloc(input_len + 1);
    if (!result) {
        return NULL;
    }
    cursor = input;

    status = regcomp(&regex, pattern, REG_EXTENDED);
    if (status) {
        free(result);
        return NULL;
    }

    while (regexec(&regex, cursor, 1, &match, 0) == 0) {
        size_t prefix_len = match.rm_so;
        if (result_len + prefix_len >= input_len) {
            free(result);
            regfree(&regex);
            return NULL;
        }
        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(result + result_len, cursor, prefix_len);
        result_len += prefix_len;
        cursor += match.rm_eo;
    }

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    remaining_len = strlen(cursor);
    if (result_len + remaining_len >= input_len + 1) {
        free(result);
        regfree(&regex);
        return NULL;
    }
    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(result + result_len, cursor, remaining_len + 1);
    result_len += remaining_len;

    regfree(&regex);
    result[result_len] = '\0';
    return result;
}

int main(void) {
    const char *test1 = "HelloWorld";
    const char *test2 = "abcDEFghiJKL";
    const char *test3 = "NoLowercase";
    const char *test4 = "ALLUPPERCASE";
    const char *test5 = "Mixed123lowercaseUPPER";
    char *cleaned;

    cleaned = remove_lowercase_substrings(test1);
    if (cleaned) {
        printf("Input:    %s\nOutput:   %s\n\n", test1, cleaned);
        free(cleaned);
    }

    cleaned = remove_lowercase_substrings(test2);
    if (cleaned) {
        printf("Input:    %s\nOutput:   %s\n\n", test2, cleaned);
        free(cleaned);
    }

    cleaned = remove_lowercase_substrings(test3);
    if (cleaned) {
        printf("Input:    %s\nOutput:   %s\n\n", test3, cleaned);
        free(cleaned);
    }

    cleaned = remove_lowercase_substrings(test4);
    if (cleaned) {
        printf("Input:    %s\nOutput:   %s\n\n", test4, cleaned);
        free(cleaned);
    }

    cleaned = remove_lowercase_substrings(test5);
    if (cleaned) {
        printf("Input:    %s\nOutput:   %s\n\n", test5, cleaned);
        free(cleaned);
    }

    return 0;
}