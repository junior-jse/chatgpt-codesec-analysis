#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

char *add_spaces_before_capitals(const char *input) {
    if (input == NULL) {
        return NULL;
    }

    regex_t regex;
    const char *pattern = "([A-Z])";
    int ret = regcomp(&regex, pattern, REG_EXTENDED);
    if (ret) {
        return NULL;
    }

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t input_len = strlen(input);
    if (input_len == 0) {
        regfree(&regex);
        char *result = (char *)malloc(1);
        if (result != NULL) {
            result[0] = '\0';
        }
        return result;
    }

    size_t result_len = input_len + 1;
    const char *cursor = input;
    regmatch_t matches[2];

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

    size_t src_pos = 0;
    size_t dst_pos = 0;

    if (input[0] >= 'A' && input[0] <= 'Z') {
        result[dst_pos++] = input[src_pos++];
    }

    cursor = input + src_pos;
    while (regexec(&regex, cursor, 2, matches, 0) == 0) {
        size_t match_start = matches[0].rm_so;
        size_t match_end = matches[0].rm_eo;

        if (match_start > 0) {
            size_t bytes_to_copy = match_start;
            if (dst_pos + bytes_to_copy > result_len) {
                bytes_to_copy = result_len - dst_pos;
            }
            if (bytes_to_copy > 0) {
                /* Possible weaknesses found:
                 * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
                 */
                memcpy(result + dst_pos, cursor, bytes_to_copy);
                dst_pos += bytes_to_copy;
            }
        }

        if (dst_pos > 0 && result[dst_pos - 1] != ' ') {
            if (dst_pos < result_len) {
                result[dst_pos++] = ' ';
            }
        }

        if (match_start == 0 && src_pos == 0 && input[0] >= 'A' && input[0] <= 'Z') {
            if (dst_pos < result_len) {
                result[dst_pos++] = input[0];
            }
        } else {
            size_t bytes_to_copy = match_end - match_start;
            if (dst_pos + bytes_to_copy > result_len) {
                bytes_to_copy = result_len - dst_pos;
            }
            if (bytes_to_copy > 0) {
                /* Possible weaknesses found:
                 * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
                 */
                memcpy(result + dst_pos, cursor + match_start, bytes_to_copy);
                dst_pos += bytes_to_copy;
            }
        }

        cursor += match_end;
        src_pos += match_end;

        if (match_end == 0) {
            break;
        }
    }

    if (*cursor != '\0') {
        /* Possible weaknesses found:
         * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
         */
        size_t remaining = strlen(cursor);
        if (dst_pos + remaining > result_len) {
            remaining = result_len - dst_pos;
        }
        if (remaining > 0) {
            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
            memcpy(result + dst_pos, cursor, remaining);
            dst_pos += remaining;
        }
    }

    if (dst_pos < result_len) {
        result[dst_pos] = '\0';
    } else {
        result[result_len - 1] = '\0';
    }

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