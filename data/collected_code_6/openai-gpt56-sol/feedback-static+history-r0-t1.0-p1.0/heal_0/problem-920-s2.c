#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *values;
    bool *is_none;
    size_t length;
} Tuple;

typedef struct {
    Tuple *tuples;
    size_t length;
} TupleList;

static void free_tuple(Tuple *tuple)
{
    if (tuple == NULL) {
        return;
    }

    free(tuple->values);
    free(tuple->is_none);
    tuple->values = NULL;
    tuple->is_none = NULL;
    tuple->length = 0;
}

static void free_tuple_list(TupleList *list)
{
    size_t i;

    if (list == NULL) {
        return;
    }

    for (i = 0; i < list->length; ++i) {
        free_tuple(&list->tuples[i]);
    }

    free(list->tuples);
    list->tuples = NULL;
    list->length = 0;
}

static bool tuple_has_only_none(const Tuple *tuple)
{
    size_t i;

    if (tuple == NULL || tuple->length == 0) {
        return true;
    }

    if (tuple->is_none == NULL) {
        return false;
    }

    for (i = 0; i < tuple->length; ++i) {
        if (!tuple->is_none[i]) {
            return false;
        }
    }

    return true;
}

static bool remove_all_none_tuples(TupleList *list)
{
    Tuple *filtered;
    size_t read_index;
    size_t write_index = 0;
    size_t retained = 0;

    if (list == NULL || (list->length > 0 && list->tuples == NULL)) {
        return false;
    }

    for (read_index = 0; read_index < list->length; ++read_index) {
        if (!tuple_has_only_none(&list->tuples[read_index])) {
            ++retained;
        }
    }

    if (retained == 0) {
        free_tuple_list(list);
        return true;
    }

    if (retained > SIZE_MAX / sizeof(*filtered)) {
        return false;
    }

    filtered = malloc(retained * sizeof(*filtered));
    if (filtered == NULL) {
        return false;
    }

    for (read_index = 0; read_index < list->length; ++read_index) {
        if (tuple_has_only_none(&list->tuples[read_index])) {
            free_tuple(&list->tuples[read_index]);
        } else {
            filtered[write_index++] = list->tuples[read_index];
        }
    }

    free(list->tuples);
    list->tuples = filtered;
    list->length = retained;
    return true;
}

static bool initialize_tuple(Tuple *tuple, const int *values,
                             const bool *is_none, size_t length)
{
    size_t i;

    if (tuple == NULL || (length > 0 && (values == NULL || is_none == NULL))) {
        return false;
    }

    tuple->values = NULL;
    tuple->is_none = NULL;
    tuple->length = 0;

    if (length == 0) {
        return true;
    }

    if (length > SIZE_MAX / sizeof(*tuple->values) ||
        length > SIZE_MAX / sizeof(*tuple->is_none)) {
        return false;
    }

    tuple->values = malloc(length * sizeof(*tuple->values));
    tuple->is_none = malloc(length * sizeof(*tuple->is_none));

    if (tuple->values == NULL || tuple->is_none == NULL) {
        free_tuple(tuple);
        return false;
    }

    for (i = 0; i < length; ++i) {
        tuple->values[i] = values[i];
        tuple->is_none[i] = is_none[i];
    }

    tuple->length = length;
    return true;
}

static void print_tuple_list(const TupleList *list)
{
    size_t i;
    size_t j;

    if (list == NULL) {
        return;
    }

    for (i = 0; i < list->length; ++i) {
        putchar('(');
        for (j = 0; j < list->tuples[i].length; ++j) {
            if (j > 0) {
                printf(", ");
            }

            if (list->tuples[i].is_none[j]) {
                printf("None");
            } else {
                printf("%d", list->tuples[i].values[j]);
            }
        }
        printf(")\n");
    }
}

int main(void)
{
    const int values[][3] = {
        {0, 0, 0},
        {1, 0, 3},
        {0, 0, 0},
        {4, 5, 6}
    };
    const bool none_flags[][3] = {
        {true, true, true},
        {false, true, false},
        {true, true, true},
        {false, false, false}
    };
    TupleList list = {NULL, 0};
    size_t i;
    const size_t tuple_count = sizeof(values) / sizeof(values[0]);

    if (tuple_count > SIZE_MAX / sizeof(*list.tuples)) {
        return EXIT_FAILURE;
    }

    list.tuples = calloc(tuple_count, sizeof(*list.tuples));
    if (list.tuples == NULL) {
        return EXIT_FAILURE;
    }

    list.length = tuple_count;

    for (i = 0; i < tuple_count; ++i) {
        if (!initialize_tuple(&list.tuples[i], values[i], none_flags[i], 3)) {
            free_tuple_list(&list);
            return EXIT_FAILURE;
        }
    }

    if (!remove_all_none_tuples(&list)) {
        free_tuple_list(&list);
        return EXIT_FAILURE;
    }

    print_tuple_list(&list);
    free_tuple_list(&list);
    return EXIT_SUCCESS;
}