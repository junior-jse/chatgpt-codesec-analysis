#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 1000

typedef struct {
    char items[MAX_SIZE];
    int top;
} Stack;

void initStack(Stack *s) {
    s->top = -1;
}

int isEmpty(const Stack *s) {
    return s->top == -1;
}

int isFull(const Stack *s) {
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
    if (expression == NULL) {
        return 0;
    }

    initStack(&stack);

    for (int i = 0; expression[i] != '\0'; i++) {
        char c = expression[i];

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
    char input[MAX_SIZE];

    printf("Enter an expression: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 1;
    }

    input[strcspn(input, "\n")] = '\0';

    if (isBalanced(input)) {
        printf("Balanced\n");
    } else {
        printf("Not Balanced\n");
    }

    return 0;
}