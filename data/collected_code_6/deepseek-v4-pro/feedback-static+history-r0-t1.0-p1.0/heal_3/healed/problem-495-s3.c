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

    input_len = strlen(input);
    result = malloc(input_len + 1);
    if (!result) {
        return NULL;
    }
    *result = '\0';
    cursor = input;

    status = regcomp(&regex, pattern, REG_EXTENDED);
    if (status) {
        free(result);
        return NULL;
    }

    while (regexec(&regex, cursor, 1, &match, 0) == 0) {
        size_t prefix_len = match.rm_so;
        
        if (result_len + prefix_len >= input_len + 1) {
            free(result);
            regfree(&regex);
            return NULL;
        }
        if (prefix_len > 0) {
            memcpy(result + result_len, cursor, prefix_len);
        }
        result_len += prefix_len;
        cursor += match.rm_eo;
    }

    remaining_len = strlen(cursor);
    if (result_len + remaining_len >= input_len + 1) {
        free(result);
        regfree(&regex);
        return NULL;
    }
    if (remaining_len > 0) {
        memcpy(result + result_len, cursor, remaining_len);
    }
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