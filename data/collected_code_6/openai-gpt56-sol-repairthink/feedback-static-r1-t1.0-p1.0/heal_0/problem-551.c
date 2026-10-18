#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

typedef struct {
    int *values;
    size_t length;
} IntList;

typedef struct {
    IntList *rows;
    size_t length;
} NestedList;

static int extract_column(const NestedList *list, size_t column,
                          int **result, size_t *result_length)
{
    int *column_values;
    size_t i;

    if (list == NULL || result == NULL || result_length == NULL) {
        return -1;
    }

    *result = NULL;
    *result_length = 0;

    if (list->length == 0) {
        return 0;
    }

    if (list->rows == NULL || list->length > SIZE_MAX / sizeof(*column_values)) {
        return -1;
    }

    for (i = 0; i < list->length; ++i) {
        if (list->rows[i].values == NULL || column >= list->rows[i].length) {
            return -1;
        }
    }

    column_values = malloc(list->length * sizeof(*column_values));
    if (column_values == NULL) {
        return -1;
    }

    for (i = 0; i < list->length; ++i) {
        column_values[i] = list->rows[i].values[column];
    }

    *result = column_values;
    *result_length = list->length;
    return 0;
}

int main(void)
{
    int row0[] = {1, 2, 3};
    int row1[] = {4, 5, 6};
    int row2[] = {7, 8, 9};
    IntList rows[] = {
        {row0, sizeof(row0) / sizeof(row0[0])},
        {row1, sizeof(row1) / sizeof(row1[0])},
        {row2, sizeof(row2) / sizeof(row2[0])}
    };
    NestedList list = {rows, sizeof(rows) / sizeof(rows[0])};
    int *column = NULL;
    size_t column_length = 0;
    size_t i;

    if (extract_column(&list, 1, &column, &column_length) != 0) {
        fputs("Failed to extract column.\n", stderr);
        return EXIT_FAILURE;
    }

    for (i = 0; i < column_length; ++i) {
        if (printf("%d%s", column[i],
                   i + 1 == column_length ? "\n" : " ") < 0) {
            free(column);
            return EXIT_FAILURE;
        }
    }

    free(column);
    return EXIT_SUCCESS;
}