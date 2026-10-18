#include <stdio.h>
#include <stdlib.h>

int* extract_elements(int** matrix, int rows, int cols, const int* indices, int num_indices) {
    if (matrix == NULL || indices == NULL || num_indices <= 0 || rows <= 0 || cols <= 0 || rows * cols < num_indices) {
        return NULL;
    }

    int* result = (int*)malloc(num_indices * sizeof(int));
    if (result == NULL) {
        return NULL;
    }

    int total_elements = rows * cols;
    for (int i = 0; i < num_indices; i++) {
        if (indices[i] < 0 || indices[i] >= total_elements) {
            free(result);
            return NULL;
        }
        int r = indices[i] / cols;
        int c = indices[i] % cols;
        result[i] = matrix[r][c];
    }

    return result;
}

int main() {
    int r1[] = {10, 20, 30};
    int r2[] = {40, 50, 60};
    int* matrix[] = {r1, r2};
    int rows = 2;
    int cols = 3;

    int indices[] = {0, 2, 4, 5};
    int num_indices = 4;

    int* extracted = extract_elements(matrix, rows, cols, indices, num_indices);
    
    if (extracted != NULL) {
        for (int i = 0; i < num_indices; i++) {
            printf("%d ", extracted[i]);
        }
        printf("\n");
        free(extracted);
    } else {
        printf("Extraction failed.\n");
    }

    return 0;
}