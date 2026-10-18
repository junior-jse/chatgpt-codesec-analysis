#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

bool are_parentheses_valid(const char *str)
{
    size_t depth = 0;

    if (str == NULL) {
        return false;
    }

    for (; *str != '\0'; ++str) {
        if (*str == '(') {
            ++depth;
        } else if (*str == ')') {
            if (depth == 0) {
                return false;
            }
            --depth;
        } else {
            return false;
        }
    }

    return depth == 0;
}

int main(void)
{
    char *input = NULL;
    size_t capacity = 0;
    ssize_t length = getline(&input, &capacity, stdin);

    if (length < 0) {
        free(input);
        return EXIT_FAILURE;
    }

    if (length > 0 && input[length - 1] == '\n') {
        input[--length] = '\0';
    }

    if (length > 0 && input[length - 1] == '\r') {
        input[--length] = '\0';
    }

    puts(are_parentheses_valid(input) ? "valid" : "invalid");

    free(input);
    return EXIT_SUCCESS;
}