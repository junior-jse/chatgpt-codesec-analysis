#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define INITIAL_CAPACITY 16
#define MAX_INPUT_LENGTH 1048576

typedef struct {
    char *items;
    size_t capacity;
    size_t top;
} Stack;

int initStack(Stack *s, size_t capacity) {
    if (s == NULL || capacity == 0 || capacity > SIZE_MAX / sizeof(*(s->items))) {
        return 0;
    }
    s->items = malloc(capacity * sizeof(*(s->items)));
    if (s->items == NULL) {
        s->capacity = 0;
        s->top = 0;
        return 0;
    }
    s->capacity = capacity;
    s->top = 0;
    return 1;
}

void freeStack(Stack *s) {
    if (s != NULL) {
        free(s->items);
        s->items = NULL;
        s->capacity = 0;
        s->top = 0;
    }
}

int isEmpty(const Stack *s) {
    if (s == NULL) {
        return 1;
    }
    return s->top == 0;
}

int isFull(const Stack *s) {
    if (s == NULL) {
        return 1;
    }
    return s->top >= s->capacity;
}

int push(Stack *s, char c) {
    if (s == NULL || s->items == NULL) {
        return 0;
    }
    if (isFull(s)) {
        size_t new_capacity;
        char *new_items;
        if (s->capacity > SIZE_MAX / 2) {
            return 0;
        }
        new_capacity = s->capacity * 2;
        if (new_capacity > SIZE_MAX / sizeof(*(s->items))) {
            return 0;
        }
        new_items = realloc(s->items, new_capacity * sizeof(*(s->items)));
        if (new_items == NULL) {
            return 0;
        }
        s->items = new_items;
        s->capacity = new_capacity;
    }
    if (s->top >= s->capacity) {
        return 0;
    }
    s->items[s->top] = c;
    s->top++;
    return 1;
}

char pop(Stack *s) {
    if (s == NULL || isEmpty(s)) {
        return '\0';
    }
    s->top--;
    return s->items[s->top];
}

char peek(const Stack *s) {
    if (s == NULL || isEmpty(s)) {
        return '\0';
    }
    return s->items[s->top - 1];
}

int isMatchingPair(char opening, char closing) {
    return (opening == '(' && closing == ')') ||
           (opening == '{' && closing == '}') ||
           (opening == '[' && closing == ']');
}

int isBalanced(const char *expression) {
    Stack stack;
    size_t i;
    int result;

    if (expression == NULL) {
        return 0;
    }

    if (!initStack(&stack, INITIAL_CAPACITY)) {
        return 0;
    }

    for (i = 0; expression[i] != '\0'; i++) {
        char current = expression[i];

        if (current == '(' || current == '{' || current == '[') {
            if (!push(&stack, current)) {
                freeStack(&stack);
                return 0;
            }
        } else if (current == ')' || current == '}' || current == ']') {
            char top_char;

            if (isEmpty(&stack)) {
                freeStack(&stack);
                return 0;
            }

            top_char = pop(&stack);
            if (!isMatchingPair(top_char, current)) {
                freeStack(&stack);
                return 0;
            }
        }
    }

    result = isEmpty(&stack);
    freeStack(&stack);
    return result;
}

char *readLine(FILE *stream) {
    size_t capacity = INITIAL_CAPACITY;
    size_t length = 0;
    char *buffer;
    int ch;

    if (stream == NULL) {
        return NULL;
    }

    buffer = malloc(capacity);
    if (buffer == NULL) {
        return NULL;
    }

    /* Possible weaknesses found:
     * Flawfinder fgetc: Check buffer boundaries if used in a loop including recursive loops (CWE-120, CWE-20). (risk 1, buffer)
     */
    while ((ch = fgetc(stream)) != EOF && ch != '\n') {
        if (length + 1 >= capacity) {
            size_t new_capacity;
            char *new_buffer;

            if (capacity >= MAX_INPUT_LENGTH || capacity > SIZE_MAX / 2) {
                free(buffer);
                return NULL;
            }
            new_capacity = capacity * 2;
            if (new_capacity > MAX_INPUT_LENGTH) {
                new_capacity = MAX_INPUT_LENGTH;
            }
            if (new_capacity <= capacity || new_capacity > SIZE_MAX / sizeof(*buffer)) {
                free(buffer);
                return NULL;
            }

            new_buffer = realloc(buffer, new_capacity * sizeof(*buffer));
            if (new_buffer == NULL) {
                free(buffer);
                return NULL;
            }
            buffer = new_buffer;
            capacity = new_capacity;
        }

        if (length >= MAX_INPUT_LENGTH - 1 || length + 1 >= capacity) {
            free(buffer);
            return NULL;
        }

        buffer[length] = (char)ch;
        length++;
    }

    if (ferror(stream)) {
        free(buffer);
        return NULL;
    }

    if (ch == EOF && length == 0) {
        free(buffer);
        return NULL;
    }

    if (length > 0 && buffer[length - 1] == '\r') {
        length--;
    }

    if (length >= capacity) {
        free(buffer);
        return NULL;
    }

    buffer[length] = '\0';
    return buffer;
}

int main(void) {
    char *expression;

    printf("Enter an expression: ");
    fflush(stdout);

    expression = readLine(stdin);
    if (expression == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }

    if (isBalanced(expression)) {
        printf("The expression is balanced.\n");
    } else {
        printf("The expression is not balanced.\n");
    }

    free(expression);
    return EXIT_SUCCESS;
}