#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 1000

typedef struct {
    char items[MAX_SIZE];
    int top;
} Stack;

static void initStack(Stack *s) {
    if (s != NULL) {
        s->top = -1;
    }
}

static int isEmpty(const Stack *s) {
    return (s == NULL) || (s->top == -1);
}

static int isFull(const Stack *s) {
    return (s != NULL) && (s->top == MAX_SIZE - 1);
}

static int push(Stack *s, char c) {
    if (s == NULL || isFull(s)) {
        return 0;
    }
    s->top++;
    s->items[s->top] = c;
    return 1;
}

static int pop(Stack *s, char *c) {
    if (s == NULL || c == NULL || isEmpty(s)) {
        return 0;
    }
    *c = s->items[s->top];
    s->top--;
    return 1;
}

static int peek(const Stack *s, char *c) {
    if (s == NULL || c == NULL || isEmpty(s)) {
        return 0;
    }
    *c = s->items[s->top];
    return 1;
}

static int isMatchingPair(char opening, char closing) {
    return (opening == '(' && closing == ')') ||
           (opening == '{' && closing == '}') ||
           (opening == '[' && closing == ']');
}

static int isBalanced(const char *expression) {
    Stack stack;
    size_t i;
    size_t len;

    if (expression == NULL) {
        return 0;
    }

    len = strnlen(expression, MAX_SIZE + 1);
    if (len >= MAX_SIZE) {
        return 0;
    }

    initStack(&stack);

    for (i = 0; i < len; i++) {
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
    char input[MAX_SIZE + 1];
    int result;
    size_t len;

    printf("Enter an expression: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 1;
    }

    len = strcspn(input, "\n");
    if (len < sizeof(input)) {
        input[len] = '\0';
    }

    len = strnlen(input, MAX_SIZE + 1);
    if (len >= MAX_SIZE) {
        printf("Not Balanced\n");
        return 0;
    }

    result = isBalanced(input);

    if (result) {
        printf("Balanced\n");
    } else {
        printf("Not Balanced\n");
    }

    return 0;
}