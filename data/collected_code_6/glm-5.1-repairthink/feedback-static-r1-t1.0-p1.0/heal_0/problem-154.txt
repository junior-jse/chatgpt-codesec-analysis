#include <stdio.h>
#include <stdlib.h>

int* extract_elements(int** matrix, int rows, const int* cols, const int* indices, int indices_len, int* out_len) {
    if (matrix == NULL || cols == NULL || indices == NULL || out_len == NULL) {
        return NULL;
    }

    int total = 0;
    for (int i = 0; i < indices_len; i++) {
        if (indices[i] < 0) {
            return NULL;
        }
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < indices_len; j++) {
            if (indices[j] < cols[i]) {
                total++;
            }
        }
    }

    if (total == 0) {
        *out_len = 0;
        return NULL;
    }

    int* result = (int*)malloc(total * sizeof(int));
    if (result == NULL) {
        return NULL;
    }

    int k = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < indices_len; j++) {
            if (indices[j] < cols[i]) {
                result[k++] = matrix[i][indices[j]];
            }
        }
    }

    *out_len = k;
    return result;
}

int main(void) {
    int r0[] = {1, 2, 3, 4};
    int r1[] = {5, 6, 7};
    int r2[] = {8, 9};

    int rows = 3;
    int cols[] = {4, 3, 2};
    int* matrix[] = {r0, r1, r2};

    int indices[] = {1, 3};
    int indices_len = 2;

    int out_len = 0;
    int* extracted = extract_elements(matrix, rows, cols, indices, indices_len, &out_len);

    if (extracted != NULL && out_len > 0) {
        for (int i = 0; i < out_len; i++) {
            printf("%d ", extracted[i]);
        }
        printf("\n");
        free(extracted);
    }

    return 0;
}