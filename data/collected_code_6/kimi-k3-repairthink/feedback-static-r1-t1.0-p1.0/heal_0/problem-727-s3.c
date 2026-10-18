#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

char* remove_non_alnum(const char* input) {
    regex_t regex;
    char* result;
    const char* cursor;
    regmatch_t match;
    size_t result_len = 0;
    size_t input_len;
    int ret;
    
    if (input == NULL) {
        return NULL;
    }
    
    input_len = strlen(input);
    result = malloc(input_len + 1);
    if (result == NULL) {
        return NULL;
    }
    
    ret = regcomp(&regex, "[A-Za-z0-9]", REG_EXTENDED);
    if (ret != 0) {
        free(result);
        return NULL;
    }
    
    cursor = input;
    while (*cursor != '\0') {
        ret = regexec(&regex, cursor, 1, &match, 0);
        if (ret == 0 && match.rm_so == 0) {
            result[result_len++] = cursor[0];
        }
        cursor++;
    }
    result[result_len] = '\0';
    
    regfree(&regex);
    return result;
}

int main(void) {
    const char* test_string = "Hello, World! 123 @#$%";
    char* cleaned;
    
    cleaned = remove_non_alnum(test_string);
    if (cleaned == NULL) {
        fprintf(stderr, "Error: Memory allocation failed or invalid input\n");
        return EXIT_FAILURE;
    }
    
    printf("Original: %s\n", test_string);
    printf("Cleaned:  %s\n", cleaned);
    
    free(cleaned);
    return EXIT_SUCCESS;
}