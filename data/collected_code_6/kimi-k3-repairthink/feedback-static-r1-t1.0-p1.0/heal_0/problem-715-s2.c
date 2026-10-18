#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

typedef struct {
    int *data;
    size_t size;
} IntTuple;

IntTuple *string_to_int_tuple(const char *str) {
    if (str == NULL) {
        return NULL;
    }

    char *copy = strdup(str);
    if (copy == NULL) {
        return NULL;
    }

    size_t capacity = 10;
    size_t count = 0;
    int *numbers = malloc(capacity * sizeof(int));
    if (numbers == NULL) {
        free(copy);
        return NULL;
    }

    char *token = strtok(copy, " ");
    while (token != NULL) {
        if (count >= capacity) {
            size_t new_capacity = capacity * 2;
            int *temp = realloc(numbers, new_capacity * sizeof(int));
            if (temp == NULL) {
                free(numbers);
                free(copy);
                return NULL;
            }
            numbers = temp;
            capacity = new_capacity;
        }

        char *endptr;
        errno = 0;
        long value = strtol(token, &endptr, 10);
        if (errno != 0 || endptr == token || *endptr != '\0' || value > INT_MAX || value < INT_MIN) {
            free(numbers);
            free(copy);
            return NULL;
        }

        numbers[count++] = (int)value;
        token = strtok(NULL, " ");
    }

    free(copy);

    IntTuple *tuple = malloc(sizeof(IntTuple));
    if (tuple == NULL) {
        free(numbers);
        return NULL;
    }

    tuple->data = numbers;
    tuple->size = count;
    return tuple;
}

void free_int_tuple(IntTuple *tuple) {
    if (tuple != NULL) {
        free(tuple->data);
        free(tuple);
    }
}

int main(void) {
    const char *input = "1 2 3 4 5";
    IntTuple *tuple = string_to_int_tuple(input);
    
    if (tuple == NULL) {
        fprintf(stderr, "Failed to convert string to tuple\n");
        return EXIT_FAILURE;
    }

    printf("Tuple: (");
    for (size_t i = 0; i < tuple->size; i++) {
        printf("%d", tuple->data[i]);
        if (i < tuple->size - 1) {
            printf(", ");
        }
    }
    printf(")\n");

    free_int_tuple(tuple);
    return EXIT_SUCCESS;
}