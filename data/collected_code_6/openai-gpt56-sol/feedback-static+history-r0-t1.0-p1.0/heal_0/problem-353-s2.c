#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct {
    size_t rows;
    size_t columns;
    int **data;
} Matrix;

static void matrix_destroy(Matrix *matrix)
{
    if (matrix == NULL) {
        return;
    }

    if (matrix->data != NULL) {
        for (size_t row = 0; row < matrix->rows; ++row) {
            free(matrix->data[row]);
        }
        free(matrix->data);
    }

    matrix->data = NULL;
    matrix->rows = 0;
    matrix->columns = 0;
}

static int matrix_create(Matrix *matrix, size_t rows, size_t columns)
{
    if (matrix == NULL || rows == 0 || columns == 0 ||
        rows > SIZE_MAX / sizeof(*matrix->data) ||
        columns > SIZE_MAX / sizeof(**matrix->data)) {
        return 0;
    }

    matrix->rows = 0;
    matrix->columns = 0;
    matrix->data = calloc(rows, sizeof(*matrix->data));

    if (matrix->data == NULL) {
        return 0;
    }

    matrix->rows = rows;
    matrix->columns = columns;

    for (size_t row = 0; row < rows; ++row) {
        matrix->data[row] = malloc(columns * sizeof(*matrix->data[row]));
        if (matrix->data[row] == NULL) {
            matrix_destroy(matrix);
            return 0;
        }
    }

    return 1;
}

static int matrix_remove_column(Matrix *matrix, size_t column)
{
    if (matrix == NULL || matrix->data == NULL ||
        column >= matrix->columns || matrix->columns <= 1) {
        return 0;
    }

    const size_t new_columns = matrix->columns - 1;
    int **new_data = calloc(matrix->rows, sizeof(*new_data));

    if (new_data == NULL) {
        return 0;
    }

    for (size_t row = 0; row < matrix->rows; ++row) {
        if (matrix->data[row] == NULL) {
            for (size_t i = 0; i < row; ++i) {
                free(new_data[i]);
            }
            free(new_data);
            return 0;
        }

        new_data[row] = malloc(new_columns * sizeof(*new_data[row]));
        if (new_data[row] == NULL) {
            for (size_t i = 0; i < row; ++i) {
                free(new_data[i]);
            }
            free(new_data);
            return 0;
        }

        size_t destination = 0;
        for (size_t source = 0; source < matrix->columns; ++source) {
            if (source != column) {
                new_data[row][destination++] = matrix->data[row][source];
            }
        }
    }

    for (size_t row = 0; row < matrix->rows; ++row) {
        free(matrix->data[row]);
    }
    free(matrix->data);

    matrix->data = new_data;
    matrix->columns = new_columns;

    return 1;
}

static void matrix_print(const Matrix *matrix)
{
    if (matrix == NULL || matrix->data == NULL) {
        return;
    }

    for (size_t row = 0; row < matrix->rows; ++row) {
        for (size_t column = 0; column < matrix->columns; ++column) {
            printf("%d%s", matrix->data[row][column],
                   column + 1 == matrix->columns ? "\n" : " ");
        }
    }
}

int main(void)
{
    Matrix matrix = {0, 0, NULL};
    const int values[][4] = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12}
    };
    const size_t column_to_remove = 1;

    if (!matrix_create(&matrix, 3, 4)) {
        fputs("Failed to create matrix.\n", stderr);
        return EXIT_FAILURE;
    }

    for (size_t row = 0; row < matrix.rows; ++row) {
        for (size_t column = 0; column < matrix.columns; ++column) {
            matrix.data[row][column] = values[row][column];
        }
    }

    if (!matrix_remove_column(&matrix, column_to_remove)) {
        fputs("Failed to remove column.\n", stderr);
        matrix_destroy(&matrix);
        return EXIT_FAILURE;
    }

    matrix_print(&matrix);
    matrix_destroy(&matrix);

    return EXIT_SUCCESS;
}