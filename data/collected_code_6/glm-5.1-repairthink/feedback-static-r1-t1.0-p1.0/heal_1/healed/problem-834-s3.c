#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>

int** generate_spiral_matrix(int n, int* return_size, int** return_column_sizes) {
    *return_size = n;
    *return_column_sizes = malloc(n * sizeof(int));
    int** matrix = malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        matrix[i] = malloc(n * sizeof(int));
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
    for (int i = 0; i < n; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

int main(int argc, const char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <n>\n", argv[0]);
        return 1;
    }

    char *endptr;
    errno = 0;
    long val = strtol(argv[1], &endptr, 10);

    if (endptr == argv[1] || *endptr != '\0') {
        fprintf(stderr, "Invalid input: not a valid integer.\n");
        return 1;
    }

    if (errno == ERANGE || val <= 0 || val > INT_MAX) {
        fprintf(stderr, "n must be a positive integer within valid range.\n");
        return 1;
    }

    int n = (int)val;

    int return_size;
    int* return_column_sizes;

    int** matrix = generate_spiral_matrix(n, &return_size, &return_column_sizes);

    for (int i = 0; i < return_size; i++) {
        for (int j = 0; j < return_column_sizes[i]; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }

    free_spiral_matrix(matrix, return_size);
    free(return_column_sizes);

    return 0;
}