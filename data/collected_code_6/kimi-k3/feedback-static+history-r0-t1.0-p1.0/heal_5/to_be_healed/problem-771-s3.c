#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 100

typedef struct {
    char *items;
    int top;
    int capacity;
} Stack;

int initStack(Stack *s, int capacity) {
    if (s == NULL || capacity <= 0) {
        return 0;
    }
    s->items = (char *)malloc((size_t)capacity * sizeof(char));
    if (s->items == NULL) {
        return 0;
    }
    s->top = -1;
    s->capacity = capacity;
    return 1;
}

void destroyStack(Stack *s) {
    if (s != NULL) {
        free(s->items);
        s->items = NULL;
        s->top = -1;
        s->capacity = 0;
    }
}

int isEmpty(const Stack *s) {
    if (s == NULL) {
        return 1;
    }
    return s->top == -1;
}

int isFull(const Stack *s) {
    if (s == NULL) {
        return 1;
    }
    return s->top == s->capacity - 1;
}

int push(Stack *s, char c) {
    if (s == NULL || isFull(s)) {
        return 0;
    }
    s->items[++(s->top)] = c;
    return 1;
}

char pop(Stack *s) {
    if (s == NULL || isEmpty(s)) {
        return '\0';
    }
    return s->items[(s->top)--];
}

char peek(const Stack *s) {
    if (s == NULL || isEmpty(s)) {
        return '\0';
    }
    return s->items[s->top];
}

int isMatchingPair(char opening, char closing) {
    return (opening == '(' && closing == ')') ||
           (opening == '{' && closing == '}') ||
           (opening == '[' && closing == ']');
}

int isBalanced(const char *expression, size_t max_len) {
    Stack stack;
    size_t i;
    size_t len;
    int result;
    
    if (expression == NULL || max_len == 0) {
        return 0;
    }
    
    len = strnlen(expression, max_len);
    if (len == max_len) {
        return 0;
    }
    
    if (!initStack(&stack, (int)(len + 1))) {
        return 0;
    }
    
    for (i = 0; i < len; i++) {
        char current = expression[i];
        
        if (current == '(' || current == '{' || current == '[') {
            if (!push(&stack, current)) {
                destroyStack(&stack);
                return 0;
            }
        } else if (current == ')' || current == '}' || current == ']') {
            char top_char;
            
            if (isEmpty(&stack)) {
                destroyStack(&stack);
                return 0;
            }
            
            top_char = pop(&stack);
            if (!isMatchingPair(top_char, current)) {
                destroyStack(&stack);
                return 0;
            }
        }
    }
    
    result = isEmpty(&stack);
    destroyStack(&stack);
    return result;
}

int main(void) {
    char *expression = NULL;
    size_t buffer_size = MAX_SIZE + 2;
    size_t input_len;
    int c;
    int too_long = 0;
    
    expression = (char *)malloc(buffer_size * sizeof(char));
    if (expression == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter an expression: ");
    
    input_len = 0;
    /* Possible weaknesses found:
     * Flawfinder getchar: Check buffer boundaries if used in a loop including recursive loops (CWE-120, CWE-20). (risk 1, buffer)
     */
    while (input_len < buffer_size - 1 && (c = getchar()) != EOF && c != '\n') {
        expression[input_len++] = (char)c;
    }
    
    if (c != EOF && c != '\n' && input_len >= buffer_size - 1) {
        too_long = 1;
    }
    
    if (too_long) {
        int ch;
        /* Possible weaknesses found:
         * Flawfinder getchar: Check buffer boundaries if used in a loop including recursive loops (CWE-120, CWE-20). (risk 1, buffer)
         */
        while ((ch = getchar()) != '\n' && ch != EOF);
        fprintf(stderr, "Input too long\n");
        free(expression);
        return EXIT_FAILURE;
    }
    
    expression[input_len] = '\0';
    
    if (isBalanced(expression, buffer_size)) {
        printf("The expression is balanced.\n");
    } else {
        printf("The expression is not balanced.\n");
    }
    
    free(expression);
    return EXIT_SUCCESS;
}