#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

char* add_spaces_before_capitals(const char* input) {
    if (input == NULL) {
        return NULL;
    }

    size_t input_len = strnlen(input, SIZE_MAX);
    if (input_len == SIZE_MAX) {
        return NULL;
    }

    regex_t regex;
    int ret = regcomp(&regex, "[A-Z]", REG_EXTENDED);
    if (ret != 0) {
        return NULL;
    }

    size_t max_result_len = input_len * 2 + 1;
    char* result = (char*)malloc(max_result_len);
    if (result == NULL) {
        regfree(&regex);
        return NULL;
    }

    size_t result_index = 0;
    const char* cursor = input;
    regmatch_t match;

    /* Possible weaknesses found:
     *  Assuming that condition 'result_index<max_result_len-1' is not redundant
     */
    while (*cursor != '\0' && result_index < max_result_len - 1) {
        if (regexec(&regex, cursor, 1, &match, 0) == 0) {
            if (match.rm_so == 0 && cursor != input) {
                /* Possible weaknesses found:
                 *  Condition 'result_index<max_result_len-1' is always true [knownConditionTrueFalse]
                 *  Condition 'result_index<max_result_len-1' is always true
                 */
                if (result_index < max_result_len - 1) {
                    result[result_index++] = ' ';
                }
            }
            size_t copy_len = (match.rm_so == 0) ? 1 : match.rm_so;
            if (copy_len > max_result_len - result_index - 1) {
                copy_len = max_result_len - result_index - 1;
            }
            if (copy_len > 0) {
                /* Possible weaknesses found:
                 * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
                 */
                memcpy(result + result_index, cursor, copy_len);
                result_index += copy_len;
            }
            cursor += copy_len;
        } else {
            size_t remaining = strnlen(cursor, max_result_len - result_index);
            if (remaining > max_result_len - result_index - 1) {
                remaining = max_result_len - result_index - 1;
            }
            if (remaining > 0) {
                /* Possible weaknesses found:
                 * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
                 */
                memcpy(result + result_index, cursor, remaining);
                result_index += remaining;
            }
            break;
        }
    }

    result[result_index] = '\0';
    regfree(&regex);
    return result;
}

int main(void) {
    const char* test_str = "HelloWorldThisIsATest";
    char* result = add_spaces_before_capitals(test_str);
    
    if (result != NULL) {
        printf("Input: %s\n", test_str);
        printf("Output: %s\n", result);
        free(result);
    } else {
        printf("Error processing string\n");
    }
    
    return 0;
}