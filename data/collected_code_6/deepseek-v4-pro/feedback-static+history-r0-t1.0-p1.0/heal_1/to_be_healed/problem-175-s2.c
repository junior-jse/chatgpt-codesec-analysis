#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    char *data;
    size_t top;
    size_t capacity;
} Stack;

static bool stack_init(Stack *stack, size_t capacity) {
    stack->data = (char *)malloc(capacity * sizeof(char));
    if (stack->data == NULL) {
        return false;
    }
    stack->top = 0;
    stack->capacity = capacity;
    return true;
}

static void stack_destroy(Stack *stack) {
    free(stack->data);
    stack->data = NULL;
    stack->top = 0;
    stack->capacity = 0;
}

static bool stack_push(Stack *stack, char value) {
    if (stack->top >= stack->capacity) {
        size_t new_capacity = stack->capacity * 2;
        char *new_data = (char *)realloc(stack->data, new_capacity * sizeof(char));
        if (new_data == NULL) {
            return false;
        }
        stack->data = new_data;
        stack->capacity = new_capacity;
    }
    stack->data[stack->top++] = value;
    return true;
}

static bool stack_pop(Stack *stack, char *value) {
    if (stack->top == 0) {
        return false;
    }
    *value = stack->data[--stack->top];
    return true;
}

/* Possible weaknesses found:
 * Flawfinder open: Check when opening files - can an attacker redirect it (via symlinks), force the opening of special file type (e.g., device files), move things around to create a race condition, control its ancestors, or change its contents? (CWE-362). (risk 2, misc)
 */
static bool is_matching_pair(char open, char close) {
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
    if (len == 0) {
        return true;
    }
    
    Stack stack;
    if (!stack_init(&stack, len)) {
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
            /* Possible weaknesses found:
             * Flawfinder open: Check when opening files - can an attacker redirect it (via symlinks), force the opening of special file type (e.g., device files), move things around to create a race condition, control its ancestors, or change its contents? (CWE-362). (risk 2, misc)
             */
            char open;
            /* Possible weaknesses found:
             * Flawfinder open: Check when opening files - can an attacker redirect it (via symlinks), force the opening of special file type (e.g., device files), move things around to create a race condition, control its ancestors, or change its contents? (CWE-362). (risk 2, misc)
             */
            if (!stack_pop(&stack, &open) || !is_matching_pair(open, ch)) {
                stack_destroy(&stack);
                return false;
            }
        } else {
            stack_destroy(&stack);
            return false;
        }
    }
    
    bool is_empty = (stack.top == 0);
    stack_destroy(&stack);
    return is_empty;
}

int main(void) {
    const char *test_cases[] = {
        "()",
        "()[]{}",
        "(]",
        "([)]",
        "{[]}",
        "(((",
        ")))",
        "",
        "((()))",
        "{[()]}",
        "abc",
        NULL
    };
    
    for (int i = 0; test_cases[i] != NULL; i++) {
        printf("\"%s\": %s\n", test_cases[i], 
               is_valid_parentheses(test_cases[i]) ? "valid" : "invalid");
    }
    
    return 0;
}