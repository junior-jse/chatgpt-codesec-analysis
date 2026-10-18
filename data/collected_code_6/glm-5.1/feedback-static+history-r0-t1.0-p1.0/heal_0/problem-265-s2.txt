#include <stdio.h>
#include <stdlib.h>

int** split_list(const int* list, size_t list_len, size_t n, size_t* out_rows, size_t** out_cols) {
    if (list == NULL || out_rows == NULL || out_cols == NULL || n == 0) {
        return NULL;
    }

    size_t rows = (list_len + n - 1) / n;
    int** result = (int**)malloc(rows * sizeof(int*));
    if (result == NULL) {
        return NULL;
    }

    size_t* col_sizes = (size_t*)malloc(rows * sizeof(size_t));
    if (col_sizes == NULL) {
        free(result);
        return NULL;
    }

    for (size_t i = 0; i < rows; i++) {
        size_t start = i * n;
        size_t remaining = list_len - start;
        size_t current_col_size = remaining < n ? remaining : n;
        
        col_sizes[i] = current_col_size;
        result[i] = (int*)malloc(current_col_size * sizeof(int));
        
        if (result[i] == NULL) {
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            free(col_sizes);
            return NULL;
        }

        for (size_t k = 0; k < current_col_size; k++) {
            result[i][k] = list[start + k];
        }
    }

    *out_rows = rows;
    *out_cols = col_sizes;

    return result;
}

void free_split_list(int** list, size_t rows, size_t* col_sizes) {
    if (list == NULL) {
        return;
    }
    for (size_t i = 0; i < rows; i++) {
        free(list[i]);
    }
    free(list);
    free(col_sizes);
}

int main() {
    int data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    size_t data_len = sizeof(data) / sizeof(data[0]);
    size_t n = 3;
    size_t rows = 0;
    size_t* cols = NULL;

    int** result = split_list(data, data_len, n, &rows, &cols);
    
    if (result == NULL) {
        return 1;
    }

    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols[i]; j++) {
            printf("%d ", result[i][j]);
        }
        printf("\n");
    }

    free_split_list(result, rows, cols);

    return 0;
}