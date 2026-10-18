#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct {
    const int *values;
    size_t length;
} IntRow;

static int extract_column(const IntRow *rows, size_t row_count, size_t column,
                          int **result, size_t *result_count)
{
    int *output;
    size_t i;

    if (result == NULL || result_count == NULL) {
        return -1;
    }

    *result = NULL;
    *result_count = 0;

    if (row_count == 0) {
        return 0;
    }

    if (rows == NULL || row_count > SIZE_MAX / sizeof(*output)) {
        return -1;
    }

    for (i = 0; i < row_count; ++i) {
        if (rows[i].values == NULL || column >= rows[i].length) {
            return -1;
        }
    }

    output = malloc(row_count * sizeof(*output));
    if (output == NULL) {
        return -1;
    }

    for (i = 0; i < row_count; ++i) {
        output[i] = rows[i].values[column];
    }

    *result = output;
    *result_count = row_count;
    return 0;
}

int main(void)
{
    static const int row0[] = {1, 2, 3};
    static const int row1[] = {4, 5, 6};
    static const int row2[] = {7, 8, 9};
    static const IntRow rows[] = {
        {row0, sizeof(row0) / sizeof(row0[0])},
        {row1, sizeof(row1) / sizeof(row1[0])},
        {row2, sizeof(row2) / sizeof(row2[0])}
    };
    int *column_values = NULL;
    size_t value_count = 0;
    size_t i;
    int status;

    status = extract_column(
        rows,
        sizeof(rows) / sizeof(rows[0]),
        1,
        &column_values,
        &value_count
    );

    if (status != 0) {
        fputs("Failed to extract column.\n", stderr);
        return EXIT_FAILURE;
    }

    for (i = 0; i < value_count; ++i) {
        if (printf("%d%s", column_values[i],
                   i + 1 == value_count ? "\n" : " ") < 0) {
            free(column_values);
            return EXIT_FAILURE;
        }
    }

    free(column_values);
    return EXIT_SUCCESS;
}