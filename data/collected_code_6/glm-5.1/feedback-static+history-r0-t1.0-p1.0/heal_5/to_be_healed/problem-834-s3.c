#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int** generate_spiral_matrix(int n, int* return_size, int** return_column_sizes) {
    *return_size = n;
    *return_column_sizes = (int*)malloc(n * sizeof(int));
    if (*return_column_sizes == NULL) {
        *return_size = 0;
        return NULL;
    }
    
    int** matrix = (int**)malloc(n * sizeof(int*));
    if (matrix == NULL) {
        free(*return_column_sizes);
        *return_column_sizes = NULL;
        *return_size = 0;
        return NULL;
    }
    
    for (int i = 0; i < n; i++) {
        matrix[i] = (int*)malloc(n * sizeof(int));
        if (matrix[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(matrix[j]);
            }
            free(matrix);
            free(*return_column_sizes);
            *return_column_sizes = NULL;
            *return_size = 0;
            return NULL;
        }
        (*return_column_sizes)[i] = n;
    }

    int top = 0, bottom = n - 1, left = 0, right = n - 1;
    int num = 1;

    while (top <= bottom && left <= right) {
        for (int i = left; i <= right; i++) {
            matrix[top][i] = num * num;
            num++;
        }
        top++;

        for (int i = top; i <= bottom; i++) {
            matrix[i][right] = num * num;
            num++;
        }
        right--;

        if (top <= bottom) {
            for (int i = right; i >= left; i--) {
                matrix[bottom][i] = num * num;
                num++;
            }
            bottom--;
        }

        if (left <= right) {
            for (int i = bottom; i >= top; i--) {
                matrix[i][left] = num * num;
                num++;
            }
            left++;
        }
    }

    return matrix;
}

void free_spiral_matrix(int** matrix, int n) {
    if (matrix == NULL) return;
    for (int i = 0; i < n; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

/* Possible weaknesses found:
 *  Parameter 'argv' can be declared as const array [constParameter]
 */
int main(int argc, char* const argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <n>\n", argv[0]);
        return 1;
    }

    char* endptr;
    long val = strtol(argv[1], &endptr, 10);
    if (*endptr != '\0' || val <= 0 || val > INT_MAX) {
        fprintf(stderr, "n must be a positive integer within valid range.\n");
        return 1;
    }
    int n = (int)val;

    int return_size;
    int* return_column_sizes;

    int** matrix = generate_spiral_matrix(n, &return_size, &return_column_sizes);

    if (matrix != NULL && return_column_sizes != NULL) {
        for (int i = 0; i < return_size; i++) {
            for (int j = 0; j < return_column_sizes[i]; j++) {
                printf("%d ", matrix[i][j]);
            }
            printf("\n");
        }
        free_spiral_matrix(matrix, return_size);
        free(return_column_sizes);
    } else {
        fprintf(stderr, "Memory allocation failed.\n");
        return 1;
    }

    return 0;
}