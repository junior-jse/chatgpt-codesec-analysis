#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

bool isValidParentheses(const char *s, size_t len) {
    char *stack = malloc(len + 1);
    if (stack == NULL) return false;
    
    int top = -1;
    for (size_t i = 0; i < len; i++) {
        char c = s[i];
        if (c == '(' || c == '[' || c == '{') {
            stack[++top] = c;
        } else if (c == ')' || c == ']' || c == '}') {
            if (top == -1) {
                free(stack);
                return false;
            }
            char expected = stack[top--];
            if ((c == ')' && expected != '(') ||
                (c == ']' && expected != '[') ||
                (c == '}' && expected != '{')) {
                free(stack);
                return false;
            }
        } else {
            free(stack);
            return false;
        }
    }
    
    bool result = (top == -1);
    free(stack);
    return result;
}

int main(void) {
    size_t bufsize = 4096;
    char *input = malloc(bufsize);
    if (input == NULL) return 1;
    
    printf("Enter a string of parentheses: ");
    if (fgets(input, bufsize, stdin) == NULL) {
        free(input);
        return 1;
    }
    
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[--len] = '\0';
    }
    
    if (isValidParentheses(input, len)) {
        printf("Valid\n");
    } else {
        printf("Invalid\n");
    }
    
    free(input);
    return 0;
}