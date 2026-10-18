#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *values;
    size_t size;
} IntTuple;

static int compare_ints(const void *left, const void *right)
{
    const int a = *(const int *)left;
    const int b = *(const int *)right;

    return (a > b) - (a < b);
}

static int tuple_to_set(const IntTuple *tuple, IntTuple *set)
{
    int *values;
    size_t unique_count;

    if (tuple == NULL || set == NULL || (tuple->size > 0U && tuple->values == NULL)) {
        return -1;
    }

    set->values = NULL;
    set->size = 0U;

    if (tuple->size == 0U) {
        return 0;
    }

    if (tuple->size > SIZE_MAX / sizeof(*values)) {
        return -1;
    }

    values = malloc(tuple->size * sizeof(*values));
    if (values == NULL) {
        return -1;
    }

    for (size_t i = 0U; i < tuple->size; ++i) {
        values[i] = tuple->values[i];
    }

    qsort(values, tuple->size, sizeof(*values), compare_ints);

    unique_count = 1U;
    for (size_t i = 1U; i < tuple->size; ++i) {
        if (values[i] != values[unique_count - 1U]) {
            values[unique_count] = values[i];
            ++unique_count;
        }
    }

    set->values = values;
    set->size = unique_count;
    return 0;
}

static void free_tuple(IntTuple *tuple)
{
    if (tuple != NULL) {
        free(tuple->values);
        tuple->values = NULL;
        tuple->size = 0U;
    }
}

int main(void)
{
    int input_values[] = {4, 2, 7, 2, 4, 9, 7, 1};
    IntTuple tuple = {
        input_values,
        sizeof(input_values) / sizeof(input_values[0])
    };
    IntTuple set = {NULL, 0U};

    if (tuple_to_set(&tuple, &set) != 0) {
        fputs("Failed to convert tuple to set.\n", stderr);
        return EXIT_FAILURE;
    }

    putchar('{');
    for (size_t i = 0U; i < set.size; ++i) {
        if (i > 0U) {
            fputs(", ", stdout);
        }
        printf("%d", set.values[i]);
    }
    puts("}");

    free_tuple(&set);
    return EXIT_SUCCESS;
}