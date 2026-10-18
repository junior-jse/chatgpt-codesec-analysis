#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <regex.h>

char* add_spaces_before_capitals(const char* input, size_t input_len) {
    regex_t regex;
    regmatch_t match;
    const char* cursor;
    const char* nul;
    char* work;
    char* result;
    size_t work_len;
    size_t work_capacity;
    size_t result_len;
    size_t result_capacity;
    int ret;

    if (input == NULL) {
        return NULL;
    }

    if (input_len > (SIZE_MAX - 3) / 4) {
        return NULL;
    }

    ret = regcomp(&regex, "[A-Z]", REG_EXTENDED);
    if (ret != 0) {
        return NULL;
    }

    result_capacity = input_len * 2 + 1;
    result = malloc(result_capacity);
    if (result == NULL) {
        regfree(&regex);
        return NULL;
    }

    /* Possible weaknesses found:
     *  Assignment 'work_capacity=input_len+1', assigned value is greater than 0
     */
    work_capacity = input_len + 1;
    /* Possible weaknesses found:
     *  Condition 'work_capacity==0' is always false [knownConditionTrueFalse]
     *  Condition 'work_capacity==0' is always false
     */
    if (work_capacity == 0) {
        free(result);
        regfree(&regex);
        return NULL;
    }

    work = malloc(work_capacity);
    if (work == NULL) {
        free(result);
        regfree(&regex);
        return NULL;
    }

    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(work, input, input_len);
    work[input_len] = '\0';

    nul = memchr(work, '\0', work_capacity);
    if (nul == NULL) {
        free(work);
        free(result);
        regfree(&regex);
        return NULL;
    }
    work_len = (size_t)(nul - work);

    result[0] = '\0';
    result_len = 0;
    cursor = work;

    while (*cursor != '\0') {
        size_t prefix_len;
        size_t needed;
        size_t remaining;

        ret = regexec(&regex, cursor, 1, &match, 0);

        if (ret == REG_NOMATCH) {
            remaining = work_len - (size_t)(cursor - work);

            if (remaining > SIZE_MAX - result_len - 1) {
                free(work);
                free(result);
                regfree(&regex);
                return NULL;
            }

            needed = result_len + remaining + 1;

            if (needed > result_capacity) {
                char* temp = realloc(result, needed);
                if (temp == NULL) {
                    free(work);
                    free(result);
                    regfree(&regex);
                    return NULL;
                }
                result = temp;
                result_capacity = needed;
            }

            if (result_len >= result_capacity ||
                remaining + 1 > result_capacity - result_len) {
                free(work);
                free(result);
                regfree(&regex);
                return NULL;
            }

            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
            memcpy(result + result_len, cursor, remaining + 1);
            /* Possible weaknesses found:
             *  Value stored to 'result_len' is never read [deadcode.DeadStores]
             *  Variable 'result_len' is assigned a value that is never used. [unreadVariable]
             */
            result_len += remaining;
            break;
        } else if (ret != 0) {
            free(work);
            free(result);
            regfree(&regex);
            return NULL;
        }

        remaining = work_len - (size_t)(cursor - work);

        if (match.rm_so < 0 || match.rm_eo <= match.rm_so ||
            (size_t)match.rm_eo > remaining) {
            free(work);
            free(result);
            regfree(&regex);
            return NULL;
        }

        prefix_len = (size_t)match.rm_so;

        if (prefix_len > SIZE_MAX - result_len - 3) {
            free(work);
            free(result);
            regfree(&regex);
            return NULL;
        }

        needed = result_len + prefix_len + 3;

        if (needed > result_capacity) {
            size_t new_capacity;
            char* temp;

            if (needed > SIZE_MAX / 2) {
                free(work);
                free(result);
                regfree(&regex);
                return NULL;
            }

            new_capacity = needed * 2;
            temp = realloc(result, new_capacity);
            if (temp == NULL) {
                free(work);
                free(result);
                regfree(&regex);
                return NULL;
            }
            result = temp;
            result_capacity = new_capacity;
        }

        if (result_len >= result_capacity ||
            prefix_len + 3 > result_capacity - result_len) {
            free(work);
            free(result);
            regfree(&regex);
            return NULL;
        }

        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(result + result_len, cursor, prefix_len);
        result_len += prefix_len;

        if (result_len > 0) {
            result[result_len++] = ' ';
        }

        result[result_len++] = cursor[(size_t)match.rm_so];
        result[result_len] = '\0';

        cursor += match.rm_eo;
    }

    free(work);
    regfree(&regex);
    return result;
}

int main(void) {
    const char test_string[] = "HelloWorldThisIsATest";
    char* result;

    result = add_spaces_before_capitals(test_string, sizeof(test_string) - 1);

    if (result == NULL) {
        fprintf(stderr, "Error processing string\n");
        return EXIT_FAILURE;
    }

    printf("Original: %s\n", test_string);
    printf("Modified: %s\n", result);

    free(result);

    return EXIT_SUCCESS;
}