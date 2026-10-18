#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int *elements;
    size_t size;
} Tuple;

Tuple *string_to_tuple(const char *str) {
    if (str == NULL) {
        return NULL;
    }

    size_t len = strlen(str);
    size_t capacity = len > 0 ? len : 1;
    int *buffer = (int *)malloc(capacity * sizeof(int));
    if (buffer == NULL) {
        return NULL;
    }

    size_t count = 0;
    const char *ptr = str;

    while (*ptr != '\0') {
        while (*ptr == ' ' || *ptr == ',') {
            ptr++;
        }

        if (*ptr == '\0') {
            break;
        }

        char *endptr;
        long val = strtol(ptr, &endptr, 10);

        if (endptr == ptr) {
            break;
        }

        if (count >= capacity) {
            capacity *= 2;
            int *temp = (int *)realloc(buffer, capacity * sizeof(int));
            if (temp == NULL) {
                free(buffer);
                return NULL;
            }
            buffer = temp;
        }

        buffer[count] = (int)val;
        count++;
        ptr = endptr;
    }

    if (count == 0) {
        free(buffer);
        return NULL;
    }

    int *final_buffer = (int *)realloc(buffer, count * sizeof(int));
    if (final_buffer == NULL) {
        Tuple *tuple = (Tuple *)malloc(sizeof(Tuple));
        if (tuple == NULL) {
            free(buffer);
            return NULL;
        }
        tuple->elements = buffer;
        tuple->size = count;
        return tuple;
    }

    Tuple *tuple = (Tuple *)malloc(sizeof(Tuple));
    if (tuple == NULL) {
        free(final_buffer);
        return NULL;
    }

    tuple->elements = final_buffer;
    tuple->size = count;

    return tuple;
}

void free_tuple(Tuple *tuple) {
    if (tuple != NULL) {
        free(tuple->elements);
        free(tuple);
    }
}

int main(void) {
    const char *str = "10, 20, 30, 40, 50";
    Tuple *tuple = string_to_tuple(str);

    if (tuple == NULL) {
        return EXIT_FAILURE;
    }

    printf("(");
    for (size_t i = 0; i < tuple->size; i++) {
        printf("%d", tuple->elements[i]);
        if (i < tuple->size - 1) {
            printf(", ");
        }
    }
    printf(")\n");

    free_tuple(tuple);

    return EXIT_SUCCESS;
}