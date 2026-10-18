#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

static bool are_parentheses_valid(const char *text)
{
    size_t balance = 0U;

    if (text == NULL) {
        return false;
    }

    for (size_t i = 0U; text[i] != '\0'; ++i) {
        if (text[i] == '(') {
            if (balance == SIZE_MAX) {
                return false;
            }
            ++balance;
        } else if (text[i] == ')') {
            if (balance == 0U) {
                return false;
            }
            --balance;
        } else {
            return false;
        }
    }

    return balance == 0U;
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <parentheses>\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (are_parentheses_valid(argv[1])) {
        puts("Valid");
        return EXIT_SUCCESS;
    }

    puts("Invalid");
    return EXIT_FAILURE;
}