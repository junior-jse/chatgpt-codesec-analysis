#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

static int **create_array(size_t rows, size_t columns)
{
    int **array = NULL;
    int *data = NULL;
    size_t i;

    if (rows == 0 || columns == 0 ||
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

    for (i = 0; i < rows; ++i) {
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
    const size_t rows = 4;
    const size_t columns = 5;
    int **array = create_array(rows, columns);
    size_t i;
    size_t j;

    if (array == NULL) {
        fputs("Failed to create array.\n", stderr);
        return EXIT_FAILURE;
    }

    for (i = 0; i < rows; ++i) {
        for (j = 0; j < columns; ++j) {
            array[i][j] = (int)(i * columns + j);
        }
    }

    for (i = 0; i < rows; ++i) {
        for (j = 0; j < columns; ++j) {
            if (printf("%d%c", array[i][j],
                       j + 1 == columns ? '\n' : ' ') < 0) {
                destroy_array(array);
                return EXIT_FAILURE;
            }
        }
    }

    destroy_array(array);
    return EXIT_SUCCESS;
}