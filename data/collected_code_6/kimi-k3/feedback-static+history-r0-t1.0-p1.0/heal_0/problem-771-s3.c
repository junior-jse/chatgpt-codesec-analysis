#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 100

typedef struct {
    char items[MAX_SIZE];
    int top;
} Stack;

void initStack(Stack *s) {
    if (s != NULL) {
        s->top = -1;
    }
}

int isEmpty(Stack *s) {
    if (s == NULL) {
        return 1;
    }
    return s->top == -1;
}

int isFull(Stack *s) {
    if (s == NULL) {
        return 1;
    }
    return s->top == MAX_SIZE - 1;
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

char peek(Stack *s) {
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

int isBalanced(const char *expression) {
    Stack stack;
    size_t i;
    size_t len;
    
    if (expression == NULL) {
        return 0;
    }
    
    initStack(&stack);
    len = strlen(expression);
    
    for (i = 0; i < len; i++) {
        char current = expression[i];
        
        if (current == '(' || current == '{' || current == '[') {
            if (!push(&stack, current)) {
                return 0;
            }
        } else if (current == ')' || current == '}' || current == ']') {
            char top_char;
            
            if (isEmpty(&stack)) {
                return 0;
            }
            
            top_char = pop(&stack);
            if (!isMatchingPair(top_char, current)) {
                return 0;
            }
        }
    }
    
    return isEmpty(&stack);
}

int main(void) {
    char expression[MAX_SIZE];
    
    printf("Enter an expression: ");
    if (fgets(expression, sizeof(expression), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    expression[strcspn(expression, "\n")] = '\0';
    
    if (isBalanced(expression)) {
        printf("The expression is balanced.\n");
    } else {
        printf("The expression is not balanced.\n");
    }
    
    return EXIT_SUCCESS;
}