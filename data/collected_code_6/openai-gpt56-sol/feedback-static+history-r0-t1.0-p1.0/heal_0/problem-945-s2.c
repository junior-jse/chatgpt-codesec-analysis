#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *values;
    size_t length;
} Tuple;

static int compare_ints(const void *a, const void *b)
{
    const int lhs = *(const int *)a;
    const int rhs = *(const int *)b;

    return (lhs > rhs) - (lhs < rhs);
}

static int tuple_to_set(const Tuple *tuple, Tuple *set)
{
    int *values;
    size_t unique_count;

    if (tuple == NULL || set == NULL ||
        (tuple->length > 0U && tuple->values == NULL)) {
        return -1;
    }

    set->values = NULL;
    set->length = 0U;

    if (tuple->length == 0U) {
        return 0;
    }

    if (tuple->length > SIZE_MAX / sizeof(*values)) {
        return -1;
    }

    values = malloc(tuple->length * sizeof(*values));
    if (values == NULL) {
        return -1;
    }

    for (size_t i = 0U; i < tuple->length; ++i) {
        values[i] = tuple->values[i];
    }

    qsort(values, tuple->length, sizeof(*values), compare_ints);

    unique_count = 1U;
    for (size_t i = 1U; i < tuple->length; ++i) {
        if (values[i] != values[unique_count - 1U]) {
            values[unique_count] = values[i];
            ++unique_count;
        }
    }

    set->values = values;
    set->length = unique_count;
    return 0;
}

static void free_tuple(Tuple *tuple)
{
    if (tuple != NULL) {
        free(tuple->values);
        tuple->values = NULL;
        tuple->length = 0U;
    }
}

static int print_tuple(const Tuple *tuple)
{
    if (tuple == NULL || (tuple->length > 0U && tuple->values == NULL)) {
        return -1;
    }

    if (putchar('{') == EOF) {
        return -1;
    }

    for (size_t i = 0U; i < tuple->length; ++i) {
        if (printf(i == 0U ? "%d" : ", %d", tuple->values[i]) < 0) {
            return -1;
        }
    }

    if (puts("}") == EOF) {
        return -1;
    }

    return 0;
}

int main(void)
{
    int input_values[] = {4, 2, 7, 2, 4, 9, 7, 1};
    const Tuple tuple = {
        input_values,
        sizeof(input_values) / sizeof(input_values[0])
    };
    Tuple set = {NULL, 0U};

    if (tuple_to_set(&tuple, &set) != 0) {
        fputs("Failed to convert tuple to set.\n", stderr);
        return EXIT_FAILURE;
    }

    if (print_tuple(&set) != 0) {
        free_tuple(&set);
        fputs("Failed to print set.\n", stderr);
        return EXIT_FAILURE;
    }

    free_tuple(&set);
    return EXIT_SUCCESS;
}