#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int **data;
    int rows;
    int cols;
} Matrix;

typedef struct {
    int *data;
    int len;
} Array;

Matrix create_matrix(int rows, int cols) {
    Matrix m;
    m.rows = rows;
    m.cols = cols;
    m.data = (int **)malloc(rows * sizeof(int *));
    if (m.data == NULL) {
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < rows; i++) {
        m.data[i] = (int *)malloc(cols * sizeof(int));
        if (m.data[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(m.data[j]);
            }
            free(m.data);
            exit(EXIT_FAILURE);
        }
    }
    return m;
}

void free_matrix(Matrix m) {
    for (int i = 0; i < m.rows; i++) {
        free(m.data[i]);
    }
    free(m.data);
}

Array extract_elements(Matrix m, int row_step, int col_step) {
    if (row_step <= 0 || col_step <= 0) {
        Array empty = {NULL, 0};
        return empty;
    }
    int count = 0;
    for (int i = 0; i < m.rows; i += row_step) {
        for (int j = 0; j < m.cols; j += col_step) {
            count++;
        }
    }
    Array arr;
    arr.len = count;
    arr.data = (int *)malloc(count * sizeof(int));
    if (arr.data == NULL) {
        exit(EXIT_FAILURE);
    }
    int idx = 0;
    for (int i = 0; i < m.rows; i += row_step) {
        for (int j = 0; j < m.cols; j += col_step) {
            arr.data[idx++] = m.data[i][j];
        }
    }
    return arr;
}

int main() {
    int rows = 4;
    int cols = 4;
    Matrix m = create_matrix(rows, cols);
    int val = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            m.data[i][j] = val++;
        }
    }
    Array arr = extract_elements(m, 2, 2);
    for (int i = 0; i < arr.len; i++) {
        printf("%d ", arr.data[i]);
    }
    printf("\n");
    free(arr.data);
    free_matrix(m);
    return 0;
}