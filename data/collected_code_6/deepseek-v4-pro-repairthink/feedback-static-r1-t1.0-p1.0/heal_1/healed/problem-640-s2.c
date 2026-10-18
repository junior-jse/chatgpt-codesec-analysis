#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static char *remove_parenthesis_area(const char *input) {
    if (input == NULL) {
        return NULL;
    }

    size_t out_len = 0;
    int depth = 0;
    for (const char *p = input; *p != '\0'; ++p) {
        if (*p == '(') {
            depth++;
        } else if (*p == ')') {
            if (depth > 0) {
                depth--;
            } else {
                out_len++;
            }
        } else {
            if (depth == 0) {
                out_len++;
            }
        }
    }

    char *result = malloc(out_len + 1);
    if (result == NULL) {
        return NULL;
    }

    size_t write_pos = 0;
    depth = 0;
    for (const char *p = input; *p != '\0'; ++p) {
        char c = *p;
        if (c == '(') {
            depth++;
        } else if (c == ')') {
            if (depth > 0) {
                depth--;
            } else {
                result[write_pos++] = c;
            }
        } else {
            if (depth == 0) {
                result[write_pos++] = c;
            }
        }
    }

    result[write_pos] = '\0';
    return result;
}

int main(void) {
    const char *test_cases[] = {
        "Hello (world) example",
        "Nested ((parenthesis)) test",
        "No parenthesis here",
        "Start (paren) end",
        "Unmatched (parenthesis",
        "Unmatched parenthesis)",
        "Empty () test",
        "(Only parenthesis)",
        ""
    };

    size_t num_tests = sizeof(test_cases) / sizeof(test_cases[0]);

    for (size_t i = 0; i < num_tests; i++) {
        char *cleaned = remove_parenthesis_area(test_cases[i]);
        if (cleaned != NULL) {
            printf("Input:    \"%s\"\n", test_cases[i]);
            printf("Output:   \"%s\"\n", cleaned);
            printf("\n");
            free(cleaned);
        } else {
            fprintf(stderr, "Memory allocation failed for test %zu\n", i);
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}