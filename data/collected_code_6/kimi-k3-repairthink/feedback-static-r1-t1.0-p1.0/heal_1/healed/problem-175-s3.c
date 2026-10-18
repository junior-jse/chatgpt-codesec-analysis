#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX_INPUT_LENGTH 4096

static size_t bounded_strlen(const char *str, size_t max_len) {
    size_t len = 0;

    if (str == NULL) {
        return 0;
    }

    while (len < max_len && str[len] != '\0') {
        len++;
    }

    return len;
}

bool is_valid_parentheses(const char *str) {
    if (str == NULL) {
        return false;
    }

    int balance = 0;
    size_t len = bounded_strlen(str, MAX_INPUT_LENGTH);

    if (len == MAX_INPUT_LENGTH) {
        return false;
    }

    for (size_t i = 0; i < len; i++) {
        if (str[i] == '(') {
            balance++;
        } else if (str[i] == ')') {
            balance--;
            if (balance < 0) {
                return false;
            }
        }
    }

    return balance == 0;
}

int main(void) {
    const char *test_cases[] = {
        "()",
        "(())",
        "()()",
        "(()",
        "())",
        "((()))",
        "(()())",
        "",
        "a(b)c",
        ")(",
        NULL
    };

    for (int i = 0; test_cases[i] != NULL; i++) {
        printf("String: \"%s\" -> %s\n",
               test_cases[i],
               is_valid_parentheses(test_cases[i]) ? "Valid" : "Invalid");
    }

    return 0;
}