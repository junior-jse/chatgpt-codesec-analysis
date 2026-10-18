#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

char **split_at_uppercase(const char *str, int *count) {
    /* Possible weaknesses found:
     *  Assuming that condition 'count==NULL' is not redundant
     */
    if (str == NULL || count == NULL) {
        /* Possible weaknesses found:
         *  Null pointer dereference
         *  Either the condition 'count==NULL' is redundant or there is possible null pointer dereference: count. [nullPointerRedundantCheck]
         */
        *count = 0;
        return NULL;
    }
    
    regex_t regex;
    regmatch_t matches[2];
    int ret;
    
    ret = regcomp(&regex, "[A-Z]", REG_EXTENDED);
    if (ret != 0) {
        *count = 0;
        return NULL;
    }
    
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    int len = strlen(str);
    int *positions = malloc((len + 1) * sizeof(int));
    if (positions == NULL) {
        regfree(&regex);
        *count = 0;
        return NULL;
    }
    
    int pos_count = 0;
    const char *p = str;
    int offset = 0;
    
    while (offset < len) {
        ret = regexec(&regex, p, 2, matches, 0);
        if (ret == 0) {
            positions[pos_count++] = offset + matches[0].rm_so;
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
    
    /* Possible weaknesses found:
     *  The scope of the variable 'start' can be reduced. [variableScope]
     */
    int start = 0;
    int result_count = 0;
    
    if (pos_count == 0) {
        result[0] = malloc((len + 1) * sizeof(char));
        if (result[0] == NULL) {
            free(result);
            free(positions);
            regfree(&regex);
            *count = 0;
            return NULL;
        }
        /* Possible weaknesses found:
         * Flawfinder strcpy: Does not check for buffer overflows when copying to destination [MS-banned] (CWE-120). Consider using snprintf, strcpy_s, or strlcpy (warning: strncpy easily misused). (risk 4, buffer)
         */
        strcpy(result[0], str);
        result_count = 1;
    } else {
        for (int i = 0; i < pos_count; i++) {
            if (positions[i] > start) {
                int segment_len = positions[i] - start;
                result[result_count] = malloc((segment_len + 1) * sizeof(char));
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
                /* Possible weaknesses found:
                 * Flawfinder strncpy: Easily used incorrectly; doesn't always \0-terminate or check for invalid pointers [MS-banned] (CWE-120). (risk 1, buffer)
                 */
                strncpy(result[result_count], str + start, segment_len);
                result[result_count][segment_len] = '\0';
                result_count++;
            }
            start = positions[i];
        }
        
        if (start < len) {
            int segment_len = len - start;
            result[result_count] = malloc((segment_len + 1) * sizeof(char));
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
            /* Possible weaknesses found:
             * Flawfinder strncpy: Easily used incorrectly; doesn't always \0-terminate or check for invalid pointers [MS-banned] (CWE-120). (risk 1, buffer)
             */
            strncpy(result[result_count], str + start, segment_len);
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

void free_split_result(char **result) {
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