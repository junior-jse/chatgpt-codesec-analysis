#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

bool isValidParentheses(const char *s) {
    if (s == NULL) return false;
    
    size_t len = strnlen(s, 256);
    if (len >= 256) return false;
    
    char *stack = malloc(len + 1);
    if (stack == NULL) return false;
    
    int top = -1;
    
    for (size_t i = 0; i < len; i++) {
        char c = s[i];
        if (c == '(' || c == '[' || c == '{') {
            if ((size_t)(top + 1) >= len) {
                free(stack);
                return false;
            }
            stack[++top] = c;
        } else if (c == ')' || c == ']' || c == '}') {
            if (top == -1) {
                free(stack);
                return false;
            }
            char open_bracket = stack[top--];
            if ((c == ')' && open_bracket != '(') ||
                (c == ']' && open_bracket != '[') ||
                (c == '}' && open_bracket != '{')) {
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
    char *input = NULL;
    size_t bufsize = 0;
    
    printf("Enter a string of parentheses: ");
    ssize_t read = getline(&input, &bufsize, stdin);
    if (read == -1) {
        free(input);
        return 1;
    }
    
    if (read > 0 && input[read - 1] == '\n') {
        input[read - 1] = '\0';
        read--;
    }
    
    if (read < 0 || read > 255) {
        free(input);
        return 1;
    }
    
    if (isValidParentheses(input)) {
        printf("Valid\n");
    } else {
        printf("Invalid\n");
    }
    
    free(input);
    return 0;
}