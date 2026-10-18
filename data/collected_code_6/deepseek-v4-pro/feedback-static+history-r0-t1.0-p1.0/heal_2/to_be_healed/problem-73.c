/* Repair history (previous round):
 * - Flawfinder strncpy was replaced by memcpy, which Flawfinder flags with the same CWE (CWE-120).
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

char **split_regex(const char *str, const char *pattern, int *count) {
    regex_t regex;
    regmatch_t match;
    char **result = NULL;
    int capacity = 10;
    int size = 0;
    const char *cursor = str;
    size_t str_len;
    
    if (!str || !pattern || !count) {
        if (count) *count = 0;
        return NULL;
    }
    
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    str_len = strlen(str);
    const char *end = str + str_len;
    
    if (regcomp(&regex, pattern, REG_EXTENDED)) {
        *count = 0;
        return NULL;
    }
    
    result = (char **)malloc(capacity * sizeof(char *));
    if (!result) {
        regfree(&regex);
        *count = 0;
        return NULL;
    }
    
    while (cursor <= end && regexec(&regex, cursor, 1, &match, 0) == 0) {
        const char *token_start = cursor;
        size_t token_len = match.rm_so;
        
        if (match.rm_so == -1) {
            token_len = end - cursor;
        }
        
        if (size == capacity) {
            capacity *= 2;
            char **temp = (char **)realloc(result, capacity * sizeof(char *));
            if (!temp) {
                for (int i = 0; i < size; i++) {
                    free(result[i]);
                }
                free(result);
                regfree(&regex);
                *count = 0;
                return NULL;
            }
            result = temp;
        }
        
        result[size] = (char *)malloc((token_len + 1) * sizeof(char));
        if (!result[size]) {
            for (int i = 0; i < size; i++) {
                free(result[i]);
            }
            free(result);
            regfree(&regex);
            *count = 0;
            return NULL;
        }
        
        if (token_len > 0) {
            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
            memcpy(result[size], token_start, token_len);
        }
        result[size][token_len] = '\0';
        size++;
        
        cursor += match.rm_so + match.rm_eo;
        
        if (match.rm_so == 0 && match.rm_eo == 0) {
            cursor++;
        }
    }
    
    /* Possible weaknesses found:
     *  Assuming that condition 'cursor<end' is not redundant
     */
    if (cursor < end) {
        /* Possible weaknesses found:
         *  Assignment 'remaining_len=end-cursor', assigned value is greater than 0
         */
        size_t remaining_len = end - cursor;
        
        if (size == capacity) {
            capacity *= 2;
            char **temp = (char **)realloc(result, capacity * sizeof(char *));
            if (!temp) {
                for (int i = 0; i < size; i++) {
                    free(result[i]);
                }
                free(result);
                regfree(&regex);
                *count = 0;
                return NULL;
            }
            result = temp;
        }
        
        result[size] = (char *)malloc((remaining_len + 1) * sizeof(char));
        /* Possible weaknesses found:
         *  Assuming condition is false
         */
        if (!result[size]) {
            for (int i = 0; i < size; i++) {
                free(result[i]);
            }
            free(result);
            regfree(&regex);
            *count = 0;
            return NULL;
        }
        
        /* Possible weaknesses found:
         *  Condition 'remaining_len>0' is always true
         *  Condition 'remaining_len>0' is always true [knownConditionTrueFalse]
         */
        if (remaining_len > 0) {
            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
            memcpy(result[size], cursor, remaining_len);
        }
        result[size][remaining_len] = '\0';
        size++;
    }
    
    regfree(&regex);
    *count = size;
    return result;
}

void free_split_result(char **result, int count) {
    if (result) {
        for (int i = 0; i < count; i++) {
            free(result[i]);
        }
        free(result);
    }
}

int main() {
    const char *text = "apple,banana;cherry:date|elderberry";
    const char *pattern = "[,;:|]";
    int count = 0;
    
    char **tokens = split_regex(text, pattern, &count);
    
    if (tokens) {
        printf("Split results (%d tokens):\n", count);
        for (int i = 0; i < count; i++) {
            printf("Token %d: %s\n", i, tokens[i]);
        }
        free_split_result(tokens, count);
    } else {
        printf("Failed to split string\n");
    }
    
    return 0;
}