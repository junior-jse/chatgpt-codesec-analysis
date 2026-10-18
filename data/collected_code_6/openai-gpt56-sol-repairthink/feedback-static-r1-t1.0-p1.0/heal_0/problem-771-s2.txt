#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

static bool is_balanced(const char *expression)
{
    if (expression == NULL) {
        return false;
    }

    size_t capacity = 16U;
    size_t top = 0U;
    char *stack = malloc(capacity);

    if (stack == NULL) {
        return false;
    }

    for (const char *p = expression; *p != '\0'; ++p) {
        char current = *p;

        if (current == '(' || current == '[' || current == '{') {
            if (top == capacity) {
                if (capacity > SIZE_MAX / 2U) {
                    free(stack);
                    return false;
                }

                size_t new_capacity = capacity * 2U;
                char *new_stack = realloc(stack, new_capacity);

                if (new_stack == NULL) {
                    free(stack);
                    return false;
                }

                stack = new_stack;
                capacity = new_capacity;
            }

            stack[top++] = current;
        } else if (current == ')' || current == ']' || current == '}') {
            if (top == 0U) {
                free(stack);
                return false;
            }

            char opening = stack[--top];

            if ((current == ')' && opening != '(') ||
                (current == ']' && opening != '[') ||
                (current == '}' && opening != '{')) {
                free(stack);
                return false;
            }
        }
    }

    bool balanced = top == 0U;
    free(stack);
    return balanced;
}

int main(void)
{
    char *expression = NULL;
    size_t capacity = 0U;
    ssize_t length = getline(&expression, &capacity, stdin);

    if (length < 0) {
        free(expression);
        fprintf(stderr, "Failed to read the expression.\n");
        return EXIT_FAILURE;
    }

    bool balanced = is_balanced(expression);
    free(expression);

    puts(balanced ? "Balanced" : "Not balanced");
    return EXIT_SUCCESS;
}