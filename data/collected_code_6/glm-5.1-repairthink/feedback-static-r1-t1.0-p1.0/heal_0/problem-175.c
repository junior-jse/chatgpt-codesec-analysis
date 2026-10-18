#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int verify_parentheses(const char *str) {
    if (str == NULL) {
        return 0;
    }

    size_t len = strlen(str);
    char *stack = (char *)malloc(len + 1);
    if (stack == NULL) {
        return 0;
    }

    size_t top = 0;

    for (size_t i = 0; i < len; i++) {
        char c = str[i];
        if (c == '(' || c == '[' || c == '{') {
            stack[top++] = c;
        } else if (c == ')' || c == ']' || c == '}') {
            if (top == 0) {
                free(stack);
                return 0;
            }
            char open = stack[--top];
            if ((c == ')' && open != '(') ||
                (c == ']' && open != '[') ||
                (c == '}' && open != '{')) {
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