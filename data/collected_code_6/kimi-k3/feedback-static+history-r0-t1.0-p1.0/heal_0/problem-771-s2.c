#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
    char *data;
    size_t top;
    size_t capacity;
} Stack;

static bool stack_init(Stack *stack, size_t capacity) {
    if (stack == NULL || capacity == 0) {
        return false;
    }
    stack->data = (char *)malloc(capacity * sizeof(char));
    if (stack->data == NULL) {
        return false;
    }
    stack->top = 0;
    stack->capacity = capacity;
    return true;
}

static void stack_destroy(Stack *stack) {
    if (stack != NULL) {
        free(stack->data);
        stack->data = NULL;
        stack->top = 0;
        stack->capacity = 0;
    }
}

static bool stack_push(Stack *stack, char c) {
    if (stack == NULL || stack->top >= stack->capacity) {
        return false;
    }
    stack->data[stack->top++] = c;
    return true;
}

static bool stack_pop(Stack *stack, char *c) {
    if (stack == NULL || c == NULL || stack->top == 0) {
        return false;
    }
    *c = stack->data[--stack->top];
    return true;
}

static bool stack_is_empty(const Stack *stack) {
    return stack == NULL || stack->top == 0;
}

static bool is_opening_bracket(char c) {
    return c == '(' || c == '{' || c == '[';
}

static bool is_closing_bracket(char c) {
    return c == ')' || c == '}' || c == ']';
}

static bool brackets_match(char open, char close) {
    return (open == '(' && close == ')') ||
           (open == '{' && close == '}') ||
           (open == '[' && close == ']');
}

bool is_balanced(const char *expression) {
    if (expression == NULL) {
        return false;
    }

    size_t length = strlen(expression);
    if (length == 0) {
        return true;
    }

    Stack stack;
    if (!stack_init(&stack, length)) {
        return false;
    }

    bool balanced = true;
    for (size_t i = 0; i < length && balanced; i++) {
        char current = expression[i];
        
        if (is_opening_bracket(current)) {
            if (!stack_push(&stack, current)) {
                balanced = false;
            }
        } else if (is_closing_bracket(current)) {
            char top_element;
            if (!stack_pop(&stack, &top_element) || 
                !brackets_match(top_element, current)) {
                balanced = false;
            }
        }
    }

    if (balanced && !stack_is_empty(&stack)) {
        balanced = false;
    }

    stack_destroy(&stack);
    return balanced;
}

int main(void) {
    const char *test_expressions[] = {
        "(a + b) * (c - d)",
        "{[a + b] * (c - d)}",
        "((a + b)",
        "a + b)",
        "{[a + b] * (c - d}",
        "a + b",
        "",
        "([{}])",
        "([)]"
    };
    
    size_t num_tests = sizeof(test_expressions) / sizeof(test_expressions[0]);
    
    for (size_t i = 0; i < num_tests; i++) {
        const char *expr = test_expressions[i];
        bool result = is_balanced(expr);
        printf("Expression: \"%s\" -> %s\n", 
               expr, 
               result ? "Balanced" : "Not Balanced");
    }
    
    return 0;
}