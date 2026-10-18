#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

bool isValidParentheses(char *s) {
    if (s == NULL) return false;
    
    size_t len = strlen(s);
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
    char input[256];
    
    printf("Enter a string of parentheses: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len >= sizeof(input)) {
        return 1;
    }
    
    if (isValidParentheses(input)) {
        printf("Valid\n");
    } else {
        printf("Invalid\n");
    }
    
    return 0;
}