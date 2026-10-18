#include <stdio.h>
#include <stdlib.h>

int is_balanced(const char *expression) {
    if (expression == NULL) {
        return 0;
    }

    size_t stack_size = 128;
    char *stack = (char *)malloc(stack_size);
    if (stack == NULL) {
        return 0;
    }

    int top = -1;

    for (size_t i = 0; expression[i] != '\0'; i++) {
        char c = expression[i];
        if (c == '(' || c == '{' || c == '[') {
            if ((size_t)(top + 1) >= stack_size) {
                stack_size *= 2;
                char *temp = (char *)realloc(stack, stack_size);
                if (temp == NULL) {
                    free(stack);
                    return 0;
                }
                stack = temp;
            }
            stack[++top] = c;
        } else if (c == ')' || c == '}' || c == ']') {
            if (top == -1) {
                free(stack);
                return 0;
            }
            char top_char = stack[top--];
            if ((c == ')' && top_char != '(') ||
                (c == '}' && top_char != '{') ||
                (c == ']' && top_char != '[')) {
                free(stack);
                return 0;
            }
        }
    }

    int result = (top == -1) ? 1 : 0;
    free(stack);
    return result;
}

int main(int argc, const char * const argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <expression>\n", argv[0]);
        return 1;
    }

    if (is_balanced(argv[1])) {
        printf("Balanced\n");
    } else {
        printf("Not Balanced\n");
    }

    return 0;
}