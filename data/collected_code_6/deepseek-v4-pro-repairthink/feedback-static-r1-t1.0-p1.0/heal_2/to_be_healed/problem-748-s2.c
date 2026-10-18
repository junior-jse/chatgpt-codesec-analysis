#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

char *add_spaces_before_capitals(const char *input) {
    if (input == NULL) {
        return NULL;
    }

    regex_t regex;
    regmatch_t matches[2];
    const char *pattern = "([A-Z])";
    int ret = regcomp(&regex, pattern, REG_EXTENDED);
    if (ret) {
        return NULL;
    }

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t input_len = strlen(input);
    size_t result_len = input_len + 1;
    const char *cursor = input;

    while (regexec(&regex, cursor, 2, matches, 0) == 0) {
        if (matches[0].rm_so >= 0) {
            if (matches[0].rm_so != 0 || cursor != input) {
                result_len++;
            }
        }
        cursor += matches[0].rm_eo;
        if (matches[0].rm_eo == 0) {
            break;
        }
    }

    char *result = (char *)malloc(result_len);
    if (result == NULL) {
        regfree(&regex);
        return NULL;
    }

    size_t dst = 0;
    cursor = input;

    while (regexec(&regex, cursor, 2, matches, 0) == 0) {
        size_t match_start = matches[0].rm_so;
        size_t match_end = matches[0].rm_eo;

        if (match_start > 0) {
            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
            memcpy(result + dst, cursor, match_start);
            dst += match_start;
        }

        if (!(cursor == input && match_start == 0)) {
            if (dst > 0 && result[dst - 1] != ' ') {
                result[dst++] = ' ';
            }
        }

        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(result + dst, cursor + match_start, match_end - match_start);
        dst += match_end - match_start;

        cursor += match_end;
        if (match_end == 0) {
            break;
        }
    }

    while (*cursor != '\0') {
        result[dst++] = *cursor++;
    }

    result[dst] = '\0';
    regfree(&regex);
    return result;
}

int main(void) {
    const char *test1 = "HelloWorldExample";
    const char *test2 = "thisIsASampleString";
    const char *test3 = "already separated words";
    const char *test4 = "";
    const char *test5 = "UPPERCASE";
    const char *test6 = "noSpacesHere";

    char *result1 = add_spaces_before_capitals(test1);
    char *result2 = add_spaces_before_capitals(test2);
    char *result3 = add_spaces_before_capitals(test3);
    char *result4 = add_spaces_before_capitals(test4);
    char *result5 = add_spaces_before_capitals(test5);
    char *result6 = add_spaces_before_capitals(test6);

    printf("Input: %s\nOutput: %s\n\n", test1, result1 ? result1 : "NULL");
    printf("Input: %s\nOutput: %s\n\n", test2, result2 ? result2 : "NULL");
    printf("Input: %s\nOutput: %s\n\n", test3, result3 ? result3 : "NULL");
    printf("Input: %s\nOutput: %s\n\n", test4, result4 ? result4 : "NULL");
    printf("Input: %s\nOutput: %s\n\n", test5, result5 ? result5 : "NULL");
    printf("Input: %s\nOutput: %s\n\n", test6, result6 ? result6 : "NULL");

    free(result1);
    free(result2);
    free(result3);
    free(result4);
    free(result5);
    free(result6);

    return 0;
}