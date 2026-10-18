#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *remove_parenthesis(const char *str) {
    if (str == NULL) {
        return NULL;
    }

    size_t len = strnlen(str, SIZE_MAX);
    char *result = (char *)malloc(len + 1);
    
    if (result == NULL) {
        return NULL;
    }

    size_t j = 0;
    int depth = 0;

    for (size_t i = 0; i < len; i++) {
        if (str[i] == '(') {
            depth++;
        } else if (str[i] == ')') {
            if (depth > 0) {
                depth--;
            } else {
                result[j++] = str[i];
            }
        } else {
            if (depth == 0) {
                result[j++] = str[i];
            }
        }
    }

    result[j] = '\0';

    char *shrunk = (char *)realloc(result, j + 1);
    if (shrunk != NULL) {
        return shrunk;
    }
    
    return result;
}

int main(void) {
    const char *original = "This (is a) test (string)!";
    
    const char *modified = remove_parenthesis(original);
    
    if (modified != NULL) {
        printf("Original: %s\n", original);
        printf("Modified: %s\n", modified);
        free((void *)modified);
    }

    /* Possible weaknesses found:
     *  Calling function 'remove_parenthesis' returns 0
     *  Assignment 'null_test=remove_parenthesis(NULL)', assigned value is 0
     */
    const char *null_test = remove_parenthesis(NULL);
    
    /* Possible weaknesses found:
     *  Condition 'null_test==NULL' is always true
     *  Condition 'null_test==NULL' is always true [knownConditionTrueFalse]
     */
    if (null_test == NULL) {
        printf("NULL input handled successfully.\n");
    }

    return 0;
}