#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct {
    size_t row;
    size_t column;
} Position;

static int extract_elements(const int *matrix,
                            size_t rows,
                            size_t columns,
                            const Position *positions,
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
        (position_count != 0 && positions == NULL) ||
        (rows != 0 && columns != 0 && matrix == NULL)) {
        return -1;
    }

    if (position_count == 0) {
        return 0;
    }

    if (position_count > SIZE_MAX / sizeof(*values)) {
        return -1;
    }

    values = malloc(position_count * sizeof(*values));
    if (values == NULL) {
        return -1;
    }

    for (i = 0; i < position_count; ++i) {
        if (positions[i].row >= rows || positions[i].column >= columns) {
            free(values);
            return -1;
        }

        values[i] = matrix[positions[i].row * columns + positions[i].column];
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
    const Position positions[] = {
        {0, 1},
        {1, 3},
        {2, 0},
        {2, 2}
    };
    const size_t position_count = sizeof(positions) / sizeof(positions[0]);
    int *result = NULL;
    size_t i;

    if (extract_elements(&matrix[0][0],
                         sizeof(matrix) / sizeof(matrix[0]),
                         sizeof(matrix[0]) / sizeof(matrix[0][0]),
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