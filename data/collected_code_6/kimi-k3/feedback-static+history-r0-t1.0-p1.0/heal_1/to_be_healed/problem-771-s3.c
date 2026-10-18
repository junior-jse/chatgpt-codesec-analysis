#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 100

typedef struct {
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char items[MAX_SIZE];
    int top;
} Stack;

void initStack(Stack *s) {
    if (s != NULL) {
        s->top = -1;
    }
}

/* Possible weaknesses found:
 *  Parameter 's' can be declared as pointer to const [constParameterPointer]
 */
int isEmpty(Stack *s) {
    if (s == NULL) {
        return 1;
    }
    return s->top == -1;
}

/* Possible weaknesses found:
 *  Parameter 's' can be declared as pointer to const [constParameterPointer]
 */
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
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
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
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
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