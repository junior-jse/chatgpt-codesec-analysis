#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

typedef struct {
    int **rows;
    size_t *lengths;
    size_t row_count;
} NestedList;

int extract_column(const NestedList *list, size_t column, int **result, size_t *result_count)
{
    int *values;
    size_t count;
    size_t i;
    size_t j;

    if (list == NULL || result == NULL || result_count == NULL) {
        return -1;
    }

    *result = NULL;
    *result_count = 0;

    if (list->row_count > 0 && (list->rows == NULL || list->lengths == NULL)) {
        return -1;
    }

    count = 0;
    for (i = 0; i < list->row_count; ++i) {
        if (list->lengths[i] > 0 && list->rows[i] == NULL) {
            return -1;
        }
        if (column < list->lengths[i]) {
            ++count;
        }
    }

    if (count == 0) {
        return 0;
    }

    if (count > SIZE_MAX / sizeof(*values)) {
        return -1;
    }

    values = malloc(count * sizeof(*values));
    if (values == NULL) {
        return -1;
    }

    j = 0;
    for (i = 0; i < list->row_count; ++i) {
        if (column < list->lengths[i]) {
            values[j++] = list->rows[i][column];
        }
    }

    *result = values;
    *result_count = count;
    return 0;
}

int main(void)
{
    int row0[] = {1, 2, 3};
    int row1[] = {4, 5};
    int row2[] = {6, 7, 8, 9};
    int *rows[] = {row0, row1, row2};
    size_t lengths[] = {3, 2, 4};
    NestedList list = {rows, lengths, 3};
    int *column_values = NULL;
    size_t column_count = 0;
    size_t i;
    int status;

    status = extract_column(&list, 1, &column_values, &column_count);
    if (status != 0) {
        fputs("Failed to extract column.\n", stderr);
        return EXIT_FAILURE;
    }

    for (i = 0; i < column_count; ++i) {
        if (printf("%d%s", column_values[i], i + 1 < column_count ? " " : "\n") < 0) {
            free(column_values);
            return EXIT_FAILURE;
        }
    }

    free(column_values);
    return EXIT_SUCCESS;
}