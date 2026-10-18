#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static bool is_matching_pair(char opening, char closing)
{
    return (opening == '(' && closing == ')') ||
           (opening == '[' && closing == ']') ||
           (opening == '{' && closing == '}');
}

static bool is_balanced(const char *expression)
{
    char *stack;
    size_t length;
    size_t top = 0;
    bool balanced = true;

    if (expression == NULL) {
        return false;
    }

    length = strlen(expression);

    if (length == 0) {
        return true;
    }

    stack = malloc(length * sizeof(*stack));
    if (stack == NULL) {
        return false;
    }

    for (size_t i = 0; i < length; ++i) {
        char current = expression[i];

        if (current == '(' || current == '[' || current == '{') {
            stack[top++] = current;
        } else if (current == ')' || current == ']' || current == '}') {
            if (top == 0 || !is_matching_pair(stack[top - 1], current)) {
                balanced = false;
                break;
            }
            --top;
        }
    }

    if (top != 0) {
        balanced = false;
    }

    free(stack);
    return balanced;
}

int main(void)
{
    char *expression = NULL;
    size_t capacity = 0;
    ssize_t length;

    length = getline(&expression, &capacity, stdin);
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