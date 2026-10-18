#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>

typedef struct {
    int *elements;
    size_t size;
} IntTuple;

IntTuple* string_to_int_tuple(const char *str) {
    if (str == NULL) {
        return NULL;
    }

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t length = strlen(str);
    if (length == 0) {
        return NULL;
    }

    size_t capacity = 10;
    size_t count = 0;
    int *elements = (int *)malloc(capacity * sizeof(int));
    if (elements == NULL) {
        return NULL;
    }

    const char *ptr = str;
    while (*ptr != '\0') {
        char *end;
        errno = 0;
        long val = strtol(ptr, &end, 10);

        if (end == ptr) {
            ptr++;
            continue;
        }

        if (errno == ERANGE || val < INT_MIN || val > INT_MAX) {
            free(elements);
            return NULL;
        }

        if (count == capacity) {
            capacity *= 2;
            int *temp = (int *)realloc(elements, capacity * sizeof(int));
            if (temp == NULL) {
                free(elements);
                return NULL;
            }
            elements = temp;
        }

        elements[count++] = (int)val;
        ptr = end;
    }

    if (count == 0) {
        free(elements);
        return NULL;
    }

    IntTuple *tuple = (IntTuple *)malloc(sizeof(IntTuple));
    if (tuple == NULL) {
        free(elements);
        return NULL;
    }

    int *final_elements = (int *)realloc(elements, count * sizeof(int));
    if (final_elements == NULL) {
        free(elements);
        free(tuple);
        return NULL;
    }
    elements = final_elements;

    tuple->elements = elements;
    tuple->size = count;

    return tuple;
}

void free_int_tuple(IntTuple *tuple) {
    if (tuple != NULL) {
        free(tuple->elements);
        free(tuple);
    }
}

int main() {
    const char *input = "10, 20, 30, 40, 50";
    IntTuple *tuple = string_to_int_tuple(input);

    if (tuple == NULL) {
        fprintf(stderr, "Failed to convert string to tuple.\n");
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

    free_int_tuple(tuple);

    return EXIT_SUCCESS;
}