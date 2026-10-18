#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

char* add_spaces_before_capitals(const char* input, size_t input_len) {
    regex_t regex;
    regmatch_t match;
    const char* cursor;
    const char* end;
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
    end = input + input_len;
    
    while (cursor < end && *cursor != '\0') {
        ret = regexec(&regex, cursor, 1, &match, 0);
        
        if (ret == REG_NOMATCH) {
            remaining = 0;
            while (cursor + remaining < end && cursor[remaining] != '\0') {
                remaining++;
            }
            if (result_len + remaining + 1 > result_capacity) {
                size_t new_capacity = result_len + remaining + 1;
                char* temp = realloc(result, new_capacity);
                if (temp == NULL) {
                    free(result);
                    regfree(&regex);
                    return NULL;
                }
                result = temp;
                result_capacity = new_capacity;
            }
            if (remaining > 0) {
                if (result_len + remaining < result_capacity) {
                    memcpy(result + result_len, cursor, remaining);
                    result_len += remaining;
                } else {
                    free(result);
                    regfree(&regex);
                    return NULL;
                }
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
            size_t new_capacity = needed * 2;
            char* temp = realloc(result, new_capacity);
            if (temp == NULL) {
                free(result);
                regfree(&regex);
                return NULL;
            }
            result = temp;
            result_capacity = new_capacity;
        }
        
        if (prefix_len > 0) {
            if (result_len + prefix_len < result_capacity) {
                memcpy(result + result_len, cursor, prefix_len);
                result_len += prefix_len;
            } else {
                free(result);
                regfree(&regex);
                return NULL;
            }
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
    
    test_len = 0;
    while (test_string[test_len] != '\0') {
        test_len++;
    }
    
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