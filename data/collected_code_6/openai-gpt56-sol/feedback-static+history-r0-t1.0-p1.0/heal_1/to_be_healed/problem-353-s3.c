#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

typedef struct {
    int **rows;
    size_t row_count;
    size_t column_count;
} Matrix;

static void matrix_destroy(Matrix *matrix)
{
    size_t i;

    if (matrix == NULL) {
        return;
    }

    for (i = 0; i < matrix->row_count; ++i) {
        free(matrix->rows[i]);
    }

    free(matrix->rows);
    matrix->rows = NULL;
    matrix->row_count = 0;
    matrix->column_count = 0;
}

static int matrix_create(Matrix *matrix, size_t row_count, size_t column_count)
{
    size_t i;

    if (matrix == NULL || row_count == 0 || column_count == 0 ||
        row_count > SIZE_MAX / sizeof(*matrix->rows) ||
        column_count > SIZE_MAX / sizeof(**matrix->rows)) {
        return 0;
    }

    matrix->rows = calloc(row_count, sizeof(*matrix->rows));
    if (matrix->rows == NULL) {
        return 0;
    }

    matrix->row_count = row_count;
    matrix->column_count = column_count;

    for (i = 0; i < row_count; ++i) {
        matrix->rows[i] = malloc(column_count * sizeof(*matrix->rows[i]));
        if (matrix->rows[i] == NULL) {
            matrix_destroy(matrix);
            return 0;
        }
    }

    return 1;
}

static int remove_column(Matrix *matrix, size_t column)
{
    size_t row;
    size_t source;
    /* Possible weaknesses found:
     *  The scope of the variable 'destination' can be reduced. [variableScope]
     */
    size_t destination;
    size_t new_column_count;
    int **new_rows;

    if (matrix == NULL || matrix->rows == NULL ||
        matrix->column_count == 0 || column >= matrix->column_count) {
        return 0;
    }

    new_column_count = matrix->column_count - 1;

    if (new_column_count == 0) {
        for (row = 0; row < matrix->row_count; ++row) {
            free(matrix->rows[row]);
            matrix->rows[row] = NULL;
        }
        matrix->column_count = 0;
        return 1;
    }

    if (matrix->row_count > SIZE_MAX / sizeof(*new_rows) ||
        new_column_count > SIZE_MAX / sizeof(**new_rows)) {
        return 0;
    }

    new_rows = calloc(matrix->row_count, sizeof(*new_rows));
    if (new_rows == NULL) {
        return 0;
    }

    for (row = 0; row < matrix->row_count; ++row) {
        new_rows[row] = malloc(new_column_count * sizeof(*new_rows[row]));
        if (new_rows[row] == NULL) {
            size_t i;

            for (i = 0; i < row; ++i) {
                free(new_rows[i]);
            }
            free(new_rows);
            return 0;
        }

        destination = 0;
        for (source = 0; source < matrix->column_count; ++source) {
            if (source != column) {
                new_rows[row][destination++] = matrix->rows[row][source];
            }
        }
    }

    for (row = 0; row < matrix->row_count; ++row) {
        free(matrix->rows[row]);
    }
    free(matrix->rows);

    matrix->rows = new_rows;
    matrix->column_count = new_column_count;
    return 1;
}

static void matrix_print(const Matrix *matrix)
{
    size_t row;
    size_t column;

    if (matrix == NULL || matrix->rows == NULL) {
        return;
    }

    for (row = 0; row < matrix->row_count; ++row) {
        for (column = 0; column < matrix->column_count; ++column) {
            printf("%d%s", matrix->rows[row][column],
                   column + 1 == matrix->column_count ? "" : " ");
        }
        putchar('\n');
    }
}

int main(void)
{
    Matrix matrix = {NULL, 0, 0};
    size_t row;
    size_t column;
    size_t column_to_remove;

    if (scanf("%zu %zu", &matrix.row_count, &matrix.column_count) != 2) {
        fputs("Invalid matrix dimensions\n", stderr);
        return EXIT_FAILURE;
    }

    {
        size_t row_count = matrix.row_count;
        size_t column_count = matrix.column_count;

        matrix.rows = NULL;
        matrix.row_count = 0;
        matrix.column_count = 0;

        if (!matrix_create(&matrix, row_count, column_count)) {
            fputs("Unable to allocate matrix\n", stderr);
            return EXIT_FAILURE;
        }
    }

    for (row = 0; row < matrix.row_count; ++row) {
        for (column = 0; column < matrix.column_count; ++column) {
            if (scanf("%d", &matrix.rows[row][column]) != 1) {
                fputs("Invalid matrix element\n", stderr);
                matrix_destroy(&matrix);
                return EXIT_FAILURE;
            }
        }
    }

    if (scanf("%zu", &column_to_remove) != 1) {
        fputs("Invalid column index\n", stderr);
        matrix_destroy(&matrix);
        return EXIT_FAILURE;
    }

    if (!remove_column(&matrix, column_to_remove)) {
        fputs("Unable to remove column\n", stderr);
        matrix_destroy(&matrix);
        return EXIT_FAILURE;
    }

    matrix_print(&matrix);
    matrix_destroy(&matrix);
    return EXIT_SUCCESS;
}