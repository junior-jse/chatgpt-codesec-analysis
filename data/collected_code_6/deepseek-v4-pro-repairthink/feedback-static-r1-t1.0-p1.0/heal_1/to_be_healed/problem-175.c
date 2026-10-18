#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX_STACK_SIZE 1000

typedef struct {
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char items[MAX_STACK_SIZE];
    int top;
} Stack;

void stack_init(Stack *s) {
    s->top = -1;
}

/* Possible weaknesses found:
 *  Parameter 's' can be declared as pointer to const [constParameterPointer]
 */
bool stack_is_empty(Stack *s) {
    return s->top == -1;
}

/* Possible weaknesses found:
 *  Parameter 's' can be declared as pointer to const [constParameterPointer]
 */
bool stack_is_full(Stack *s) {
    return s->top == MAX_STACK_SIZE - 1;
}

bool stack_push(Stack *s, char value) {
    if (stack_is_full(s)) {
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

bool stack_peek(Stack *s, char *value) {
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
    if (len > MAX_STACK_SIZE) {
        return false;
    }
    
    Stack stack;
    stack_init(&stack);
    
    for (size_t i = 0; i < len; i++) {
        char ch = str[i];
        
        if (ch == '(' || ch == '[' || ch == '{') {
            if (!stack_push(&stack, ch)) {
                return false;
            }
        } else if (ch == ')' || ch == ']' || ch == '}') {
            char top_char;
            if (!stack_pop(&stack, &top_char)) {
                return false;
            }
            if (!is_matching_pair(top_char, ch)) {
                return false;
            }
        }
    }
    
    return stack_is_empty(&stack);
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