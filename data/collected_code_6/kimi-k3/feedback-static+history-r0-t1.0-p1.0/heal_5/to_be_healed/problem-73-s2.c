#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

char **split_string(const char *str, const char *pattern, int *count) {
    regex_t regex;
    regmatch_t match;
    char **result = NULL;
    char **temp = NULL;
    const char *cursor = str;
    size_t capacity = 10;
    size_t size = 0;
    size_t start = 0;
    size_t token_len;

    if (str == NULL || pattern == NULL || count == NULL) {
        return NULL;
    }

    *count = 0;

    if (regcomp(&regex, pattern, REG_EXTENDED) != 0) {
        return NULL;
    }

    result = malloc(capacity * sizeof(char *));
    if (result == NULL) {
        regfree(&regex);
        return NULL;
    }

    while (*cursor != '\0') {
        if (regexec(&regex, cursor, 1, &match, 0) == 0) {
            size_t end = start + (size_t)match.rm_so;
            
            /* Possible weaknesses found:
             *  Assuming that condition 'end>start' is not redundant
             */
            if (end > start) {
                if (size >= capacity) {
                    capacity *= 2;
                    temp = realloc(result, capacity * sizeof(char *));
                    if (temp == NULL) {
                        for (size_t i = 0; i < size; i++) {
                            free(result[i]);
                        }
                        free(result);
                        regfree(&regex);
                        return NULL;
                    }
                    result = temp;
                }

                /* Possible weaknesses found:
                 *  Assignment 'token_len=end-start', assigned value is greater than 0
                 */
                token_len = end - start;
                result[size] = malloc(token_len + 1);
                /* Possible weaknesses found:
                 *  Assuming condition is false
                 */
                if (result[size] == NULL) {
                    for (size_t i = 0; i < size; i++) {
                        free(result[i]);
                    }
                    free(result);
                    regfree(&regex);
                    return NULL;
                }

                /* Possible weaknesses found:
                 *  Comparison is wrong. Result of 'str+start' can't be 0 unless there is pointer overflow, and pointer overflow is undefined behaviour. [pointerAdditionResultNotNull]
                 *  Condition 'token_len>0' is always true
                 *  Condition 'token_len>0' is always true [knownConditionTrueFalse]
                 */
                if (token_len > 0 && (str + start) != NULL && result[size] != NULL) {
                    /* Possible weaknesses found:
                     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
                     */
                    memcpy(result[size], str + start, token_len);
                    result[size][token_len] = '\0';
                    size++;
                } else {
                    free(result[size]);
                }
            }

            start = start + (size_t)match.rm_eo;
            cursor = str + start;
        } else {
            break;
        }
    }

    if (cursor != NULL && *cursor != '\0') {
        size_t len = 0;
        const char *temp_cursor = cursor;
        while (*temp_cursor != '\0') {
            len++;
            temp_cursor++;
        }

        if (size >= capacity) {
            capacity++;
            temp = realloc(result, capacity * sizeof(char *));
            if (temp == NULL) {
                for (size_t i = 0; i < size; i++) {
                    free(result[i]);
                }
                free(result);
                regfree(&regex);
                return NULL;
            }
            result = temp;
        }

        result[size] = malloc(len + 1);
        if (result[size] == NULL) {
            for (size_t i = 0; i < size; i++) {
                free(result[i]);
            }
            free(result);
            regfree(&regex);
            return NULL;
        }

        /* Possible weaknesses found:
         *  Condition 'cursor!=NULL' is always true [knownConditionTrueFalse]
         */
        if (len > 0 && cursor != NULL && result[size] != NULL) {
            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
            memcpy(result[size], cursor, len);
            result[size][len] = '\0';
            size++;
        } else {
            free(result[size]);
        }
    }

    regfree(&regex);
    *count = (int)size;
    return result;
}

void free_split_result(char **result, int count) {
    if (result != NULL) {
        for (int i = 0; i < count; i++) {
            free(result[i]);
        }
        free(result);
    }
}

int main(void) {
    const char *input = "apple,banana;orange:grape|mango";
    const char *pattern = "[,;:|]";
    int count = 0;
    char **tokens = split_string(input, pattern, &count);

    if (tokens != NULL) {
        for (int i = 0; i < count; i++) {
            printf("%s\n", tokens[i]);
        }
        free_split_result(tokens, count);
    }

    return 0;
}