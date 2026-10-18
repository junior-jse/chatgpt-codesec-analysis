#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

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
    char input[256];
    printf("Enter a string of parentheses: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 1;
    }
    
    size_t len = 0;
    while (len < sizeof(input) && input[len] != '\0') {
        len++;
    }
    
    if (len > 0 && input[len - 1] == '\n') {
        input[--len] = '\0';
    }
    
    if (isValidParentheses(input, len)) {
        printf("Valid\n");
    } else {
        printf("Invalid\n");
    }
    
    return 0;
}