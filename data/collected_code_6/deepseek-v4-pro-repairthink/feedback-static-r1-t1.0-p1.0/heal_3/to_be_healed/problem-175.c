#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define MAX_VALID_PARENS_LEN 100000

typedef struct {
    char *items;
    int top;
    int capacity;
} Stack;

void stack_init(Stack *s, int capacity) {
    s->items = (char *)malloc(capacity * sizeof(char));
    s->top = -1;
    s->capacity = capacity;
}

void stack_destroy(Stack *s) {
    free(s->items);
    s->items = NULL;
    s->top = -1;
    s->capacity = 0;
}

bool stack_is_empty(const Stack *s) {
    return s->top == -1;
}

bool stack_is_full(const Stack *s) {
    return s->top == s->capacity - 1;
}

bool stack_push(Stack *s, char value) {
    if (stack_is_full(s) || s->items == NULL) {
        return false;
    }
    s->items[++(s->top)] = value;
    return true;
}

bool stack_pop(Stack *s, char *value) {
    if (stack_is_empty(s)) {
        return false;
    }
    *value = s->items[(s->top)--];
    return true;
}

bool stack_peek(const Stack *s, char *value) {
    if (stack_is_empty(s)) {
        return false;
    }
    *value = s->items[s->top];
    return true;
}

/* Possible weaknesses found:
 * Flawfinder open: Check when opening files - can an attacker redirect it (via symlinks), force the opening of special file type (e.g., device files), move things around to create a race condition, control its ancestors, or change its contents? (CWE-362). (risk 2, misc)
 */
bool is_matching_pair(char open, char close) {
    return (open == '(' && close == ')') ||
           (open == '[' && close == ']') ||
           (open == '{' && close == '}');
}

bool is_valid_parentheses(const char *str) {
    if (str == NULL) {
        return false;
    }

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t len = strlen(str);
    if (len > MAX_VALID_PARENS_LEN) {
        return false;
    }

    if (len == 0) {
        return true;
    }

    Stack stack;
    stack_init(&stack, (int)len);
    if (stack.items == NULL) {
        return false;
    }

    for (size_t i = 0; i < len; i++) {
        char ch = str[i];

        if (ch == '(' || ch == '[' || ch == '{') {
            if (!stack_push(&stack, ch)) {
                stack_destroy(&stack);
                return false;
            }
        } else if (ch == ')' || ch == ']' || ch == '}') {
            char top_char;
            if (!stack_pop(&stack, &top_char)) {
                stack_destroy(&stack);
                return false;
            }
            if (!is_matching_pair(top_char, ch)) {
                stack_destroy(&stack);
                return false;
            }
        }
    }

    bool result = stack_is_empty(&stack);
    stack_destroy(&stack);
    return result;
}

int main(void) {
    const char *test_cases[] = {
        "()",
        "()[]{}",
        "(]",
        "([)]",
        "{[]}",
        "",
        "(((((((((())))))))))",
        "([{}])",
        "([{})]",
        NULL
    };

    for (int i = 0; test_cases[i] != NULL; i++) {
        bool result = is_valid_parentheses(test_cases[i]);
        printf("Test %d: '%s' -> %s\n",
               i + 1,
               test_cases[i],
               result ? "valid" : "invalid");
    }

    return 0;
}