#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct {
    int **rows;
    size_t row_count;
    size_t column_count;
} Matrix;

static void free_matrix(Matrix *matrix)
{
    if (matrix == NULL) {
        return;
    }

    if (matrix->rows != NULL) {
        for (size_t i = 0; i < matrix->row_count; ++i) {
            free(matrix->rows[i]);
        }
        free(matrix->rows);
    }

    matrix->rows = NULL;
    matrix->row_count = 0;
    matrix->column_count = 0;
}

static int remove_column(Matrix *matrix, size_t column)
{
    if (matrix == NULL || column >= matrix->column_count) {
        return 0;
    }

    size_t new_column_count = matrix->column_count - 1;

    if (new_column_count == 0) {
        for (size_t i = 0; i < matrix->row_count; ++i) {
            free(matrix->rows[i]);
            matrix->rows[i] = NULL;
        }
        matrix->column_count = 0;
        return 1;
    }

    if (new_column_count > SIZE_MAX / sizeof(int)) {
        return 0;
    }

    int **new_rows = calloc(matrix->row_count, sizeof(*new_rows));
    if (matrix->row_count != 0 && new_rows == NULL) {
        return 0;
    }

    for (size_t i = 0; i < matrix->row_count; ++i) {
        new_rows[i] = malloc(new_column_count * sizeof(*new_rows[i]));
        if (new_rows[i] == NULL) {
            for (size_t j = 0; j < i; ++j) {
                free(new_rows[j]);
            }
            free(new_rows);
            return 0;
        }

        size_t destination = 0;
        for (size_t source = 0; source < matrix->column_count; ++source) {
            if (source != column) {
                new_rows[i][destination++] = matrix->rows[i][source];
            }
        }
    }

    for (size_t i = 0; i < matrix->row_count; ++i) {
        free(matrix->rows[i]);
    }
    free(matrix->rows);

    matrix->rows = new_rows;
    matrix->column_count = new_column_count;
    return 1;
}

int main(void)
{
    Matrix matrix = {0};

    if (scanf("%zu %zu", &matrix.row_count, &matrix.column_count) != 2 ||
        matrix.row_count == 0 ||
        matrix.column_count == 0 ||
        matrix.row_count > SIZE_MAX / sizeof(*matrix.rows) ||
        matrix.column_count > SIZE_MAX / sizeof(int)) {
        fputs("Invalid input\n", stderr);
        return EXIT_FAILURE;
    }

    matrix.rows = calloc(matrix.row_count, sizeof(*matrix.rows));
    if (matrix.rows == NULL) {
        fputs("Allocation failed\n", stderr);
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < matrix.row_count; ++i) {
        matrix.rows[i] = malloc(matrix.column_count * sizeof(*matrix.rows[i]));
        if (matrix.rows[i] == NULL) {
            fputs("Allocation failed\n", stderr);
            free_matrix(&matrix);
            return EXIT_FAILURE;
        }

        for (size_t j = 0; j < matrix.column_count; ++j) {
            if (scanf("%d", &matrix.rows[i][j]) != 1) {
                fputs("Invalid input\n", stderr);
                free_matrix(&matrix);
                return EXIT_FAILURE;
            }
        }
    }

    size_t column;
    if (scanf("%zu", &column) != 1 || column >= matrix.column_count) {
        fputs("Invalid column\n", stderr);
        free_matrix(&matrix);
        return EXIT_FAILURE;
    }

    if (!remove_column(&matrix, column)) {
        fputs("Unable to remove column\n", stderr);
        free_matrix(&matrix);
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < matrix.row_count; ++i) {
        for (size_t j = 0; j < matrix.column_count; ++j) {
            if (j > 0) {
                putchar(' ');
            }
            printf("%d", matrix.rows[i][j]);
        }
        putchar('\n');
    }

    free_matrix(&matrix);
    return EXIT_SUCCESS;
}