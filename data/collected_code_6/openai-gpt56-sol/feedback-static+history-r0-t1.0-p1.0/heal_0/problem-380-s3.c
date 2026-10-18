#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

static int **create_array(size_t rows, size_t columns)
{
    int **array;
    int *data;
    size_t i;

    if (rows == 0U || columns == 0U ||
        rows > SIZE_MAX / sizeof(*array) ||
        columns > SIZE_MAX / rows ||
        rows * columns > SIZE_MAX / sizeof(*data)) {
        return NULL;
    }

    array = malloc(rows * sizeof(*array));
    if (array == NULL) {
        return NULL;
    }

    data = calloc(rows * columns, sizeof(*data));
    if (data == NULL) {
        free(array);
        return NULL;
    }

    for (i = 0U; i < rows; ++i) {
        array[i] = data + (i * columns);
    }

    return array;
}

static void destroy_array(int **array)
{
    if (array != NULL) {
        free(array[0]);
        free(array);
    }
}

int main(void)
{
    const size_t rows = 3U;
    const size_t columns = 4U;
    int **array = create_array(rows, columns);
    size_t row;
    size_t column;

    if (array == NULL) {
        fputs("Failed to create array.\n", stderr);
        return EXIT_FAILURE;
    }

    for (row = 0U; row < rows; ++row) {
        for (column = 0U; column < columns; ++column) {
            array[row][column] = (int)(row * columns + column);
        }
    }

    for (row = 0U; row < rows; ++row) {
        for (column = 0U; column < columns; ++column) {
            if (printf("%d%s", array[row][column],
                       column + 1U == columns ? "\n" : " ") < 0) {
                destroy_array(array);
                return EXIT_FAILURE;
            }
        }
    }

    destroy_array(array);
    return EXIT_SUCCESS;
}