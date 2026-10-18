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
        if (str[i] == '(' || str[i] == '{' || str[i] == '[') {
            stack[top++] = str[i];
        } else if (str[i] == ')' || str[i] == '}' || str[i] == ']') {
            if (top == 0) {
                free(stack);
                return 0;
            }
            char expected;
            if (str[i] == ')') {
                expected = '(';
            } else if (str[i] == '}') {
                expected = '{';
            } else {
                expected = '[';
            }

            if (stack[top - 1] != expected) {
                free(stack);
                return 0;
            }
            top--;
        }
    }

    int result = (top == 0);
    free(stack);
    return result;
}

int main(int argc, char * const argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <string>\n", argv[0]);
        return 1;
    }

    if (verify_parentheses(argv[1])) {
        printf("Valid\n");
    } else {
        printf("Invalid\n");
    }

    return 0;
}