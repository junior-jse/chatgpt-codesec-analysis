#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int verify_parentheses(const char *str) {
    if (str == NULL) {
        return 0;
    }

    size_t capacity = 32;
    char *stack = malloc(capacity);
    if (stack == NULL) {
        return 0;
    }

    size_t top = 0;

    for (size_t i = 0; str[i] != '\0'; i++) {
        char c = str[i];
        if (c == '(' || c == '[' || c == '{') {
            if (top == capacity) {
                capacity *= 2;
                char *temp = realloc(stack, capacity);
                if (temp == NULL) {
                    free(stack);
                    return 0;
                }
                stack = temp;
            }
            stack[top++] = c;
        } else if (c == ')' || c == ']' || c == '}') {
            if (top == 0) {
                free(stack);
                return 0;
            }
            char top_char = stack[--top];
            if ((c == ')' && top_char != '(') ||
                (c == ']' && top_char != '[') ||
                (c == '}' && top_char != '{')) {
                free(stack);
                return 0;
            }
        }
    }

    int result = (top == 0);
    free(stack);
    return result;
}

int main(void) {
    const char *test1 = "([{}])";
    const char *test2 = "([)]";
    const char *test3 = "(((()";
    const char *test4 = NULL;
    
    printf("%d\n", verify_parentheses(test1));
    printf("%d\n", verify_parentheses(test2));
    printf("%d\n", verify_parentheses(test3));
    printf("%d\n", verify_parentheses(test4));

    return 0;
}