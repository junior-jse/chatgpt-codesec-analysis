#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    size_t row;
    size_t column;
} Index;

bool extract_elements(const int *matrix,
                      size_t rows,
                      size_t columns,
                      const Index *indices,
                      size_t index_count,
                      int **result)
{
    int *values;
    size_t i;

    if (result == NULL) {
        return false;
    }

    *result = NULL;

    if ((rows > 0 && columns > 0 && matrix == NULL) ||
        (index_count > 0 && indices == NULL)) {
        return false;
    }

    if (rows != 0 && columns > SIZE_MAX / rows) {
        return false;
    }

    if (index_count == 0) {
        return true;
    }

    if (index_count > SIZE_MAX / sizeof(*values)) {
        return false;
    }

    values = malloc(index_count * sizeof(*values));
    if (values == NULL) {
        return false;
    }

    for (i = 0; i < index_count; ++i) {
        if (indices[i].row >= rows || indices[i].column >= columns) {
            free(values);
            return false;
        }

        values[i] = matrix[indices[i].row * columns + indices[i].column];
    }

    *result = values;
    return true;
}

int main(void)
{
    const int matrix[][4] = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12}
    };
    const Index indices[] = {
        {0, 1},
        {1, 3},
        {2, 0},
        {2, 2}
    };
    const size_t rows = sizeof(matrix) / sizeof(matrix[0]);
    const size_t columns = sizeof(matrix[0]) / sizeof(matrix[0][0]);
    const size_t index_count = sizeof(indices) / sizeof(indices[0]);
    int *result = NULL;
    size_t i;

    if (!extract_elements(&matrix[0][0],
                          rows,
                          columns,
                          indices,
                          index_count,
                          &result)) {
        fputs("Failed to extract elements.\n", stderr);
        return EXIT_FAILURE;
    }

    for (i = 0; i < index_count; ++i) {
        if (printf("%d%s", result[i], i + 1 == index_count ? "\n" : " ") < 0) {
            free(result);
            return EXIT_FAILURE;
        }
    }

    free(result);
    return EXIT_SUCCESS;
}