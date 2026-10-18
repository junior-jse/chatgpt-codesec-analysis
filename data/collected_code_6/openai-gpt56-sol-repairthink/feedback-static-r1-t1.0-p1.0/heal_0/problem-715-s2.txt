#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *values;
    size_t length;
} IntTuple;

void free_int_tuple(IntTuple *tuple)
{
    if (tuple != NULL) {
        free(tuple->values);
        tuple->values = NULL;
        tuple->length = 0;
    }
}

int string_to_int_tuple(const char *input, IntTuple *tuple)
{
    const char *p;
    int *values = NULL;
    size_t length = 0;
    size_t capacity = 0;

    if (input == NULL || tuple == NULL) {
        return 0;
    }

    tuple->values = NULL;
    tuple->length = 0;
    p = input;

    while (*p != '\0') {
        char *end;
        long value;

        while (isspace((unsigned char)*p) || *p == ',' ||
               *p == '(' || *p == ')' || *p == '[' || *p == ']') {
            ++p;
        }

        if (*p == '\0') {
            break;
        }

        errno = 0;
        value = strtol(p, &end, 10);

        if (end == p || errno == ERANGE || value < INT_MIN || value > INT_MAX) {
            free(values);
            return 0;
        }

        if (length == capacity) {
            size_t new_capacity;
            int *new_values;

            if (capacity == 0) {
                new_capacity = 8;
            } else {
                if (capacity > SIZE_MAX / 2) {
                    free(values);
                    return 0;
                }
                new_capacity = capacity * 2;
            }

            if (new_capacity > SIZE_MAX / sizeof(*values)) {
                free(values);
                return 0;
            }

            new_values = realloc(values, new_capacity * sizeof(*values));
            if (new_values == NULL) {
                free(values);
                return 0;
            }

            values = new_values;
            capacity = new_capacity;
        }

        values[length++] = (int)value;
        p = end;

        if (*p != '\0' && !isspace((unsigned char)*p) &&
            *p != ',' && *p != ')' && *p != ']') {
            free(values);
            return 0;
        }
    }

    tuple->values = values;
    tuple->length = length;
    return 1;
}

int main(int argc, char *argv[])
{
    IntTuple tuple;
    size_t i;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s \"1, 2, 3\"\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (!string_to_int_tuple(argv[1], &tuple)) {
        fprintf(stderr, "Invalid integer string\n");
        return EXIT_FAILURE;
    }

    putchar('(');
    for (i = 0; i < tuple.length; ++i) {
        if (i != 0) {
            printf(", ");
        }
        printf("%d", tuple.values[i]);
    }
    if (tuple.length == 1) {
        putchar(',');
    }
    puts(")");

    free_int_tuple(&tuple);
    return EXIT_SUCCESS;
}