#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 1000

typedef struct {
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char items[MAX_SIZE];
    int top;
} Stack;

void initStack(Stack *s) {
    s->top = -1;
}

/* Possible weaknesses found:
 *  Parameter 's' can be declared as pointer to const [constParameterPointer]
 */
int isEmpty(Stack *s) {
    return s->top == -1;
}

/* Possible weaknesses found:
 *  Parameter 's' can be declared as pointer to const [constParameterPointer]
 */
int isFull(Stack *s) {
    return s->top == MAX_SIZE - 1;
}

int push(Stack *s, char c) {
    if (isFull(s)) {
        return 0;
    }
    s->items[++(s->top)] = c;
    return 1;
}

int pop(Stack *s, char *c) {
    if (isEmpty(s)) {
        return 0;
    }
    *c = s->items[(s->top)--];
    return 1;
}

int peek(Stack *s, char *c) {
    if (isEmpty(s)) {
        return 0;
    }
    *c = s->items[s->top];
    return 1;
}

int isMatchingPair(char opening, char closing) {
    return (opening == '(' && closing == ')') ||
           (opening == '{' && closing == '}') ||
           (opening == '[' && closing == ']');
}

int isBalanced(const char *expression) {
    Stack stack;
    /* Possible weaknesses found:
     *  The scope of the variable 'c' can be reduced. [variableScope]
     */
    char c;
    int i;
    size_t len;

    if (expression == NULL) {
        return 0;
    }

    initStack(&stack);
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    len = strlen(expression);

    for (i = 0; i < (int)len; i++) {
        c = expression[i];

        if (c == '(' || c == '{' || c == '[') {
            if (!push(&stack, c)) {
                return 0;
            }
        } else if (c == ')' || c == '}' || c == ']') {
            char topChar;
            if (!pop(&stack, &topChar)) {
                return 0;
            }
            if (!isMatchingPair(topChar, c)) {
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
    char input[MAX_SIZE];
    int result;

    printf("Enter an expression: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 1;
    }

    input[strcspn(input, "\n")] = '\0';

    result = isBalanced(input);

    if (result) {
        printf("Balanced\n");
    } else {
        printf("Not Balanced\n");
    }

    return 0;
}