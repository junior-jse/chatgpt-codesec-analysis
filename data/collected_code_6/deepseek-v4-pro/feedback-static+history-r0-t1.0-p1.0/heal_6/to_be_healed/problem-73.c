#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <limits.h>

static char **resize_result(char **result, int *capacity, int size) {
    int new_capacity;
    char **temp;
    
    if (*capacity > 0 && *capacity > (INT_MAX / 2)) {
        return NULL;
    }
    
    new_capacity = *capacity * 2;
    
    if (new_capacity <= *capacity) {
        return NULL;
    }
    
    temp = (char **)realloc(result, (size_t)new_capacity * sizeof(char *));
    if (!temp) {
        return NULL;
    }
    
    *capacity = new_capacity;
    return temp;
}

static void cleanup(char **result, int size, regex_t *regex) {
    if (result) {
        int i;
        for (i = 0; i < size; i++) {
            free(result[i]);
        }
        free(result);
    }
    if (regex) {
        regfree(regex);
    }
}

char **split_regex(const char *str, const char *pattern, int *count) {
    regex_t regex;
    regmatch_t match;
    char **result = NULL;
    int capacity = 10;
    int size = 0;
    const char *cursor;
    size_t str_len;
    const char *end;
    
    if (!str || !pattern || !count) {
        if (count) *count = 0;
        return NULL;
    }
    
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    str_len = strlen(str);
    if (str_len == 0) {
        *count = 0;
        return NULL;
    }
    
    cursor = str;
    end = str + str_len;
    
    if (regcomp(&regex, pattern, REG_EXTENDED)) {
        *count = 0;
        return NULL;
    }
    
    result = (char **)malloc((size_t)capacity * sizeof(char *));
    if (!result) {
        regfree(&regex);
        *count = 0;
        return NULL;
    }
    
    while (cursor <= end && regexec(&regex, cursor, 1, &match, 0) == 0) {
        const char *token_start = cursor;
        size_t token_len;
        
        if (match.rm_so == -1) {
            break;
        }
        
        token_len = (size_t)match.rm_so;
        
        if (size == capacity) {
            char **temp = resize_result(result, &capacity, size);
            if (!temp) {
                cleanup(result, size, &regex);
                *count = 0;
                return NULL;
            }
            result = temp;
        }
        
        result[size] = (char *)malloc(token_len + 1);
        if (!result[size]) {
            cleanup(result, size, &regex);
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
        
        cursor += (size_t)match.rm_so + (size_t)match.rm_eo;
        
        if (match.rm_so == 0 && match.rm_eo == 0) {
            if (cursor < end) {
                cursor++;
            } else {
                break;
            }
        }
    }
    
    if (cursor < end) {
        size_t remaining_len = (size_t)(end - cursor);
        
        if (size == capacity) {
            char **temp = resize_result(result, &capacity, size);
            if (!temp) {
                cleanup(result, size, &regex);
                *count = 0;
                return NULL;
            }
            result = temp;
        }
        
        result[size] = (char *)malloc(remaining_len + 1);
        if (!result[size]) {
            cleanup(result, size, &regex);
            *count = 0;
            return NULL;
        }
        
        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(result[size], cursor, remaining_len);
        result[size][remaining_len] = '\0';
        size++;
    }
    
    regfree(&regex);
    *count = size;
    return result;
}

void free_split_result(char **result, int count) {
    if (result) {
        int i;
        for (i = 0; i < count; i++) {
            free(result[i]);
        }
        free(result);
    }
}

int main(void) {
    const char *text = "apple,banana;cherry:date|elderberry";
    const char *pattern = "[,;:|]";
    int count = 0;
    
    char **tokens = split_regex(text, pattern, &count);
    
    if (tokens) {
        int i;
        printf("Split results (%d tokens):\n", count);
        for (i = 0; i < count; i++) {
            printf("Token %d: %s\n", i, tokens[i]);
        }
        free_split_result(tokens, count);
    } else {
        printf("Failed to split string\n");
    }
    
    return 0;
}