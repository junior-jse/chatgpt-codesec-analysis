#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <stdint.h>

static char **split_at_uppercase(const char *str, int *count) {
    if (str == NULL || count == NULL) {
        if (count != NULL) {
            *count = 0;
        }
        return NULL;
    }
    
    size_t len = strnlen(str, SIZE_MAX);
    /* Possible weaknesses found:
     *  safe_len is assigned 'len+1' here.
     */
    size_t safe_len = len + 1;
    
    /* Possible weaknesses found:
     *  Condition 'safe_len<len' is always false [knownConditionTrueFalse]
     *  Condition 'safe_len<len' is always false
     */
    if (safe_len < len) {
        *count = 0;
        return NULL;
    }
    
    regex_t regex;
    
    if (regcomp(&regex, "[A-Z]", REG_EXTENDED) != 0) {
        *count = 0;
        return NULL;
    }
    
    int *positions = malloc((len + 1) * sizeof(int));
    if (positions == NULL) {
        regfree(&regex);
        *count = 0;
        return NULL;
    }
    
    int pos_count = 0;
    const char *p = str;
    size_t offset = 0;
    regmatch_t matches[2];
    
    while (offset < len) {
        int ret = regexec(&regex, p, 2, matches, 0);
        if (ret == 0) {
            positions[pos_count++] = (int)(offset + matches[0].rm_so);
            p += matches[0].rm_eo;
            offset += matches[0].rm_eo;
        } else {
            break;
        }
    }
    
    char **result = malloc((pos_count + 2) * sizeof(char *));
    if (result == NULL) {
        free(positions);
        regfree(&regex);
        *count = 0;
        return NULL;
    }
    
    int result_count = 0;
    
    if (pos_count == 0) {
        result[0] = malloc(safe_len);
        if (result[0] == NULL) {
            free(result);
            free(positions);
            regfree(&regex);
            *count = 0;
            return NULL;
        }
        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(result[0], str, len);
        result[0][len] = '\0';
        result_count = 1;
    } else {
        int start = 0;
        
        for (int i = 0; i < pos_count; i++) {
            if (positions[i] > start) {
                size_t segment_len = (size_t)(positions[i] - start);
                /* Possible weaknesses found:
                 *  alloc_size is assigned 'segment_len+1' here.
                 */
                size_t alloc_size = segment_len + 1;
                
                /* Possible weaknesses found:
                 *  Condition 'alloc_size<=segment_len' is always false
                 *  Condition 'alloc_size<=segment_len' is always false [knownConditionTrueFalse]
                 */
                if (alloc_size <= segment_len) {
                    for (int j = 0; j < result_count; j++) {
                        free(result[j]);
                    }
                    free(result);
                    free(positions);
                    regfree(&regex);
                    *count = 0;
                    return NULL;
                }
                
                if (alloc_size > safe_len) {
                    for (int j = 0; j < result_count; j++) {
                        free(result[j]);
                    }
                    free(result);
                    free(positions);
                    regfree(&regex);
                    *count = 0;
                    return NULL;
                }
                
                result[result_count] = malloc(alloc_size);
                if (result[result_count] == NULL) {
                    for (int j = 0; j < result_count; j++) {
                        free(result[j]);
                    }
                    free(result);
                    free(positions);
                    regfree(&regex);
                    *count = 0;
                    return NULL;
                }
                
                if (start + segment_len > len) {
                    free(result[result_count]);
                    for (int j = 0; j < result_count; j++) {
                        free(result[j]);
                    }
                    free(result);
                    free(positions);
                    regfree(&regex);
                    *count = 0;
                    return NULL;
                }
                
                /* Possible weaknesses found:
                 * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
                 */
                memcpy(result[result_count], str + start, segment_len);
                result[result_count][segment_len] = '\0';
                result_count++;
            }
            start = positions[i];
        }
        
        if (start < (int)len) {
            size_t segment_len = len - start;
            /* Possible weaknesses found:
             *  alloc_size is assigned 'segment_len+1' here.
             */
            size_t alloc_size = segment_len + 1;
            
            /* Possible weaknesses found:
             *  Condition 'alloc_size<=segment_len' is always false
             *  Condition 'alloc_size<=segment_len' is always false [knownConditionTrueFalse]
             */
            if (alloc_size <= segment_len) {
                for (int j = 0; j < result_count; j++) {
                    free(result[j]);
                }
                free(result);
                free(positions);
                regfree(&regex);
                *count = 0;
                return NULL;
            }
            
            if (alloc_size > safe_len) {
                for (int j = 0; j < result_count; j++) {
                    free(result[j]);
                }
                free(result);
                free(positions);
                regfree(&regex);
                *count = 0;
                return NULL;
            }
            
            result[result_count] = malloc(alloc_size);
            if (result[result_count] == NULL) {
                for (int j = 0; j < result_count; j++) {
                    free(result[j]);
                }
                free(result);
                free(positions);
                regfree(&regex);
                *count = 0;
                return NULL;
            }
            
            if (start + segment_len > len) {
                free(result[result_count]);
                for (int j = 0; j < result_count; j++) {
                    free(result[j]);
                }
                free(result);
                free(positions);
                regfree(&regex);
                *count = 0;
                return NULL;
            }
            
            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
            memcpy(result[result_count], str + start, segment_len);
            result[result_count][segment_len] = '\0';
            result_count++;
        }
    }
    
    result[result_count] = NULL;
    *count = result_count;
    
    free(positions);
    regfree(&regex);
    return result;
}

static void free_split_result(char **result) {
    if (result == NULL) return;
    
    for (int i = 0; result[i] != NULL; i++) {
        free(result[i]);
    }
    free(result);
}

int main(void) {
    const char *test_string = "helloWorldThisIsATest";
    int count;
    
    char **parts = split_at_uppercase(test_string, &count);
    
    if (parts != NULL) {
        printf("Split result:\n");
        for (int i = 0; i < count; i++) {
            printf("%s\n", parts[i]);
        }
        free_split_result(parts);
    } else {
        printf("Failed to split string\n");
    }
    
    return 0;
}