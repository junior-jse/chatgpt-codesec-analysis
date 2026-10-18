#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

char* add_spaces_before_capitals(const char* input, size_t input_len) {
    regex_t regex;
    regmatch_t match;
    const char* cursor;
    char* result;
    size_t result_len;
    size_t result_capacity;
    size_t remaining;
    int ret;
    
    if (input == NULL) {
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
    
    result[0] = '\0';
    result_len = 0;
    cursor = input;
    
    while (*cursor != '\0') {
        ret = regexec(&regex, cursor, 1, &match, 0);
        
        if (ret == REG_NOMATCH) {
            /* Possible weaknesses found:
             * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
             */
            remaining = strlen(cursor);
            if (result_len + remaining + 1 > result_capacity) {
                char* temp = realloc(result, result_len + remaining + 1);
                if (temp == NULL) {
                    free(result);
                    regfree(&regex);
                    return NULL;
                }
                result = temp;
                /* Possible weaknesses found:
                 *  Value stored to 'result_capacity' is never read [deadcode.DeadStores]
                 *  Variable 'result_capacity' is assigned a value that is never used. [unreadVariable]
                 */
                result_capacity = result_len + remaining + 1;
            }
            if (remaining > 0) {
                /* Possible weaknesses found:
                 * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
                 */
                memcpy(result + result_len, cursor, remaining);
                result_len += remaining;
            }
            result[result_len] = '\0';
            break;
        } else if (ret != 0) {
            free(result);
            regfree(&regex);
            return NULL;
        }
        
        size_t prefix_len = match.rm_so;
        size_t needed = result_len + prefix_len + 2;
        
        if (needed > result_capacity) {
            char* temp = realloc(result, needed * 2);
            if (temp == NULL) {
                free(result);
                regfree(&regex);
                return NULL;
            }
            result = temp;
            result_capacity = needed * 2;
        }
        
        if (prefix_len > 0) {
            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
            memcpy(result + result_len, cursor, prefix_len);
            result_len += prefix_len;
        }
        
        if (result_len > 0) {
            result[result_len++] = ' ';
        }
        
        result[result_len++] = cursor[match.rm_so];
        result[result_len] = '\0';
        
        cursor += match.rm_eo;
    }
    
    regfree(&regex);
    return result;
}

int main(void) {
    const char* test_string = "HelloWorldThisIsATest";
    char* result;
    size_t test_len;
    
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    test_len = strlen(test_string);
    result = add_spaces_before_capitals(test_string, test_len);
    
    if (result == NULL) {
        fprintf(stderr, "Error processing string\n");
        return EXIT_FAILURE;
    }
    
    printf("Original: %s\n", test_string);
    printf("Modified: %s\n", result);
    
    free(result);
    
    return EXIT_SUCCESS;
}