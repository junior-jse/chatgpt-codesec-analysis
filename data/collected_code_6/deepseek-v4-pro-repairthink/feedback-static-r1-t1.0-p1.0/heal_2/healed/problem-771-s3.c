#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_SIZE 1000

typedef struct {
    char *items;
    int top;
    int capacity;
} Stack;

int initStack(Stack *s, int capacity) {
    s->items = malloc(capacity * sizeof(char));
    if (!s->items) {
        return 0;
    }
    s->capacity = capacity;
    s->top = -1;
    return 1;
}

void freeStack(Stack *s) {
    free(s->items);
    s->items = NULL;
}

int isEmpty(const Stack *s) {
    return s->top == -1;
}

int isFull(const Stack *s) {
    return s->top == s->capacity - 1;
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
    size_t len;
    int result = 1;

    if (expression == NULL) {
        return 0;
    }

    len = strlen(expression);
    if (!initStack(&stack, (int)len + 1)) {
        return 0;
    }

    for (size_t i = 0; expression[i] != '\0'; i++) {
        char c = expression[i];

        if (c == '(' || c == '{' || c == '[') {
            if (!push(&stack, c)) {
                result = 0;
                break;
            }
        } else if (c == ')' || c == '}' || c == ']') {
            char topChar;
            if (!pop(&stack, &topChar)) {
                result = 0;
                break;
            }
            if (!isMatchingPair(topChar, c)) {
                result = 0;
                break;
            }
        }
    }

    if (result && !isEmpty(&stack)) {
        result = 0;
    }

    freeStack(&stack);
    return result;
}

int main(void) {
    char input[INPUT_SIZE];

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