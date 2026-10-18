#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

static int extract_elements(const int *matrix,
                            size_t rows,
                            size_t columns,
                            const size_t (*positions)[2],
                            size_t position_count,
                            int **result)
{
    int *values;
    size_t i;

    if (result == NULL) {
        return -1;
    }

    *result = NULL;

    if ((rows != 0 && columns > SIZE_MAX / rows) ||
        (rows != 0 && columns != 0 && matrix == NULL) ||
        (position_count != 0 && positions == NULL) ||
        position_count > SIZE_MAX / sizeof(*values)) {
        return -1;
    }

    if (position_count == 0) {
        return 0;
    }

    values = malloc(position_count * sizeof(*values));
    if (values == NULL) {
        return -1;
    }

    for (i = 0; i < position_count; ++i) {
        size_t row = positions[i][0];
        size_t column = positions[i][1];

        if (row >= rows || column >= columns) {
            free(values);
            return -1;
        }

        values[i] = matrix[row * columns + column];
    }

    *result = values;
    return 0;
}

int main(void)
{
    const int matrix[][4] = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12}
    };
    const size_t positions[][2] = {
        {0, 1},
        {1, 3},
        {2, 0},
        {2, 2}
    };
    const size_t rows = sizeof(matrix) / sizeof(matrix[0]);
    const size_t columns = sizeof(matrix[0]) / sizeof(matrix[0][0]);
    const size_t position_count = sizeof(positions) / sizeof(positions[0]);
    int *result = NULL;
    size_t i;

    if (extract_elements(&matrix[0][0],
                         rows,
                         columns,
                         positions,
                         position_count,
                         &result) != 0) {
        fputs("Failed to extract elements.\n", stderr);
        return EXIT_FAILURE;
    }

    for (i = 0; i < position_count; ++i) {
        if (printf("%d%s", result[i], i + 1 == position_count ? "\n" : " ") < 0) {
            free(result);
            return EXIT_FAILURE;
        }
    }

    free(result);
    return EXIT_SUCCESS;
}