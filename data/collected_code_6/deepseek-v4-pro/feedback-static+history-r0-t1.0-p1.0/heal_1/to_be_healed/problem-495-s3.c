#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

char *remove_lowercase_substrings(const char *input) {
    regex_t regex;
    regmatch_t match;
    const char *pattern = "[a-z]+";
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t input_len = strlen(input);
    char *result = malloc(input_len + 1);
    size_t result_len = 0;
    const char *cursor = input;
    int status;

    if (!result) {
        return NULL;
    }

    status = regcomp(&regex, pattern, REG_EXTENDED);
    if (status) {
        free(result);
        return NULL;
    }

    while (regexec(&regex, cursor, 1, &match, 0) == 0) {
        size_t prefix_len = match.rm_so;
        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(result + result_len, cursor, prefix_len);
        result_len += prefix_len;
        cursor += match.rm_eo;
    }

    /* Possible weaknesses found:
     * Flawfinder strcpy: Does not check for buffer overflows when copying to destination [MS-banned] (CWE-120). Consider using snprintf, strcpy_s, or strlcpy (warning: strncpy easily misused). (risk 4, buffer)
     */
    strcpy(result + result_len, cursor);
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    result_len += strlen(cursor);

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