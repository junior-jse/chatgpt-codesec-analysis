#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

static bool is_matching_pair(char opening, char closing)
{
    return (opening == '(' && closing == ')') ||
           (opening == '[' && closing == ']') ||
           (opening == '{' && closing == '}');
}

static bool is_balanced(const char *expression)
{
    char *stack;
    size_t capacity = 16;
    size_t top = 0;

    if (expression == NULL) {
        return false;
    }

    stack = malloc(capacity);
    if (stack == NULL) {
        return false;
    }

    for (const char *current = expression; *current != '\0'; ++current) {
        if (*current == '(' || *current == '[' || *current == '{') {
            if (top == capacity) {
                size_t new_capacity;
                char *resized_stack;

                if (capacity > SIZE_MAX / 2) {
                    free(stack);
                    return false;
                }

                new_capacity = capacity * 2;
                resized_stack = realloc(stack, new_capacity);
                if (resized_stack == NULL) {
                    free(stack);
                    return false;
                }

                stack = resized_stack;
                capacity = new_capacity;
            }

            stack[top++] = *current;
        } else if (*current == ')' || *current == ']' || *current == '}') {
            if (top == 0 || !is_matching_pair(stack[top - 1], *current)) {
                free(stack);
                return false;
            }

            --top;
        }
    }

    bool balanced = (top == 0);
    free(stack);
    return balanced;
}

int main(void)
{
    char *expression = NULL;
    size_t capacity = 0;
    ssize_t length = getline(&expression, &capacity, stdin);

    if (length < 0) {
        free(expression);
        return EXIT_FAILURE;
    }

    if (length > 0 && expression[length - 1] == '\n') {
        expression[length - 1] = '\0';
    }

    puts(is_balanced(expression) ? "Balanced" : "Not balanced");

    free(expression);
    return EXIT_SUCCESS;
}