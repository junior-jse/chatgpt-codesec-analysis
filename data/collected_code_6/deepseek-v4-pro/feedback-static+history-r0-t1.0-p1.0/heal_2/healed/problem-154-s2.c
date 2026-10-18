#include <stdio.h>
#include <stdlib.h>

int* extract_elements(int** list, int rows, int cols, const int* indices, int idx_len, int** result, int* result_size) {
    if (list == NULL || indices == NULL || idx_len <= 0 || rows <= 0 || cols <= 0) {
        *result = NULL;
        *result_size = 0;
        return NULL;
    }

    *result_size = idx_len;
    *result = (int*)malloc(idx_len * sizeof(int));
    if (*result == NULL) {
        *result_size = 0;
        return NULL;
    }

    for (int i = 0; i < idx_len; i++) {
        int idx = indices[i];
        if (idx < 0 || idx >= rows * cols) {
            free(*result);
            *result = NULL;
            *result_size = 0;
            return NULL;
        }
        (*result)[i] = list[idx / cols][idx % cols];
    }

    return *result;
}

int main() {
    int rows = 3;
    int cols = 4;
    int** list = (int**)malloc(rows * sizeof(int*));
    if (list == NULL) {
        return 1;
    }

    for (int i = 0; i < rows; i++) {
        list[i] = (int*)malloc(cols * sizeof(int));
        if (list[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(list[j]);
            }
            free(list);
            return 1;
        }
        for (int j = 0; j < cols; j++) {
            list[i][j] = i * cols + j + 1;
        }
    }

    const int indices[] = {1, 5, 9, 11};
    int idx_len = 4;
    int* result = NULL;
    int result_size = 0;

    if (extract_elements(list, rows, cols, indices, idx_len, &result, &result_size) != NULL) {
        printf("Extracted elements: ");
        for (int i = 0; i < result_size; i++) {
            printf("%d ", result[i]);
        }
        printf("\n");
        free(result);
    } else {
        printf("Extraction failed\n");
    }

    for (int i = 0; i < rows; i++) {
        free(list[i]);
    }
    free(list);

    return 0;
}