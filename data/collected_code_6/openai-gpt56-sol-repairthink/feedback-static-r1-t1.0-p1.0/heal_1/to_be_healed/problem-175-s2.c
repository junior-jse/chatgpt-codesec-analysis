#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

static bool is_valid_parentheses(const char *str)
{
    size_t balance = 0;

    if (str == NULL) {
        return false;
    }

    for (size_t i = 0; str[i] != '\0'; ++i) {
        if (str[i] == '(') {
            ++balance;
        } else if (str[i] == ')') {
            if (balance == 0) {
                return false;
            }
            --balance;
        } else {
            return false;
        }
    }

    return balance == 0;
}

int main(void)
{
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char input[4096];

    if (fgets(input, sizeof input, stdin) == NULL) {
        if (ferror(stdin)) {
            fprintf(stderr, "Failed to read input.\n");
        } else {
            fprintf(stderr, "No input provided.\n");
        }
        return 1;
    }

    size_t length = 0;
    while (input[length] != '\0' && input[length] != '\n') {
        ++length;
    }

    if (input[length] == '\n') {
        input[length] = '\0';
    } else if (!feof(stdin)) {
        int ch;
        do {
            /* Possible weaknesses found:
             * Flawfinder getchar: Check buffer boundaries if used in a loop including recursive loops (CWE-120, CWE-20). (risk 1, buffer)
             */
            ch = getchar();
        } while (ch != '\n' && ch != EOF);

        if (ferror(stdin)) {
            fprintf(stderr, "Failed to read input.\n");
        } else {
            fprintf(stderr, "Input is too long.\n");
        }
        return 1;
    }

    if (printf("%s\n", is_valid_parentheses(input) ? "Valid" : "Invalid") < 0) {
        return 1;
    }

    return 0;
}