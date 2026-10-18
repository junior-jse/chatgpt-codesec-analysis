#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *values;
    size_t size;
} IntTuple;

static void free_tuple(IntTuple *tuple)
{
    if (tuple != NULL) {
        free(tuple->values);
        tuple->values = NULL;
        tuple->size = 0;
    }
}

static int parse_tuple(const char *input, IntTuple *tuple)
{
    const char *cursor;
    int *values = NULL;
    size_t size = 0;
    size_t capacity = 0;

    if (input == NULL || tuple == NULL) {
        return 0;
    }

    tuple->values = NULL;
    tuple->size = 0;
    cursor = input;

    while (*cursor != '\0') {
        char *end;
        long value;

        while (*cursor == ' ' || *cursor == '\t' || *cursor == '\n' ||
               *cursor == '\r' || *cursor == '\f' || *cursor == '\v' ||
               *cursor == ',') {
            ++cursor;
        }

        if (*cursor == '\0') {
            break;
        }

        errno = 0;
        value = strtol(cursor, &end, 10);

        if (end == cursor || errno == ERANGE || value < INT_MIN || value > INT_MAX) {
            free(values);
            return 0;
        }

        if (*end != '\0' && *end != ',' && *end != ' ' && *end != '\t' &&
            *end != '\n' && *end != '\r' && *end != '\f' && *end != '\v') {
            free(values);
            return 0;
        }

        if (size == capacity) {
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

        values[size++] = (int)value;
        cursor = end;
    }

    tuple->values = values;
    tuple->size = size;
    return 1;
}

static void print_tuple(const IntTuple *tuple)
{
    size_t i;

    putchar('(');

    for (i = 0; i < tuple->size; ++i) {
        if (i != 0) {
            fputs(", ", stdout);
        }

        printf("%d", tuple->values[i]);
    }

    if (tuple->size == 1) {
        putchar(',');
    }

    puts(")");
}

/* Possible weaknesses found:
 *  Parameter 'argv' can be declared as const array [constParameter]
 */
int main(int argc, char *argv[])
{
    IntTuple tuple;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s \"integer list\"\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (!parse_tuple(argv[1], &tuple)) {
        fputs("Invalid integer list or memory allocation failure\n", stderr);
        return EXIT_FAILURE;
    }

    print_tuple(&tuple);
    free_tuple(&tuple);

    if (ferror(stdout)) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}