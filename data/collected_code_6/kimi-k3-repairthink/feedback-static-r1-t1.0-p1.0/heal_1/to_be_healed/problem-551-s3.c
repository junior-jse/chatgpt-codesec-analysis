#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int **data;
    size_t rows;
    size_t cols;
} Matrix;

int *extract_column(const Matrix *matrix, size_t col_index, size_t *out_size)
{
    int *column;
    size_t i;

    if (matrix == NULL || matrix->data == NULL || out_size == NULL) {
        return NULL;
    }

    if (col_index >= matrix->cols) {
        return NULL;
    }

    column = (int *)malloc(matrix->rows * sizeof(int));
    if (column == NULL) {
        return NULL;
    }

    for (i = 0; i < matrix->rows; i++) {
        if (matrix->data[i] == NULL) {
            free(column);
            return NULL;
        }
        column[i] = matrix->data[i][col_index];
    }

    *out_size = matrix->rows;
    return column;
}

void free_matrix(Matrix *matrix)
{
    /* Possible weaknesses found:
     *  The scope of the variable 'i' can be reduced. [variableScope]
     */
    size_t i;

    if (matrix == NULL) {
        return;
    }

    if (matrix->data != NULL) {
        for (i = 0; i < matrix->rows; i++) {
            free(matrix->data[i]);
        }
        free(matrix->data);
    }

    matrix->data = NULL;
    matrix->rows = 0;
    matrix->cols = 0;
}

int init_matrix(Matrix *matrix, size_t rows, size_t cols)
{
    size_t i;

    if (matrix == NULL || rows == 0 || cols == 0) {
        return -1;
    }

    matrix->rows = rows;
    matrix->cols = cols;
    matrix->data = (int **)calloc(rows, sizeof(int *));
    if (matrix->data == NULL) {
        return -1;
    }

    for (i = 0; i < rows; i++) {
        matrix->data[i] = (int *)calloc(cols, sizeof(int));
        if (matrix->data[i] == NULL) {
            free_matrix(matrix);
            return -1;
        }
    }

    return 0;
}

int main(void)
{
    Matrix matrix;
    int *column;
    size_t col_size;
    size_t i;
    size_t j;
    size_t target_col = 1;

    if (init_matrix(&matrix, 3, 4) != 0) {
        fprintf(stderr, "Failed to initialize matrix\n");
        return EXIT_FAILURE;
    }

    for (i = 0; i < matrix.rows; i++) {
        for (j = 0; j < matrix.cols; j++) {
            matrix.data[i][j] = (int)(i * matrix.cols + j);
        }
    }

    column = extract_column(&matrix, target_col, &col_size);
    if (column == NULL) {
        fprintf(stderr, "Failed to extract column\n");
        free_matrix(&matrix);
        return EXIT_FAILURE;
    }

    printf("Column %zu: ", target_col);
    for (i = 0; i < col_size; i++) {
        printf("%d ", column[i]);
    }
    printf("\n");

    free(column);
    free_matrix(&matrix);

    return EXIT_SUCCESS;
}