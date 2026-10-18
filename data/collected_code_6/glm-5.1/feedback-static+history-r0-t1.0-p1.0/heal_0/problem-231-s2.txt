#include <stdio.h>
#include <stdlib.h>

int find_max_sum(int **triangle, int n) {
    if (triangle == NULL || n <= 0) {
        return 0;
    }
    for (int i = n - 2; i >= 0; i--) {
        if (triangle[i] == NULL || triangle[i + 1] == NULL) {
            return 0;
        }
        for (int j = 0; j <= i; j++) {
            if (triangle[i + 1][j] > triangle[i + 1][j + 1]) {
                triangle[i][j] += triangle[i + 1][j];
            } else {
                triangle[i][j] += triangle[i + 1][j + 1];
            }
        }
    }
    return triangle[0][0];
}

int main(void) {
    int n = 4;
    int **triangle = (int **)malloc(n * sizeof(int *));
    if (triangle == NULL) {
        return 1;
    }
    for (int i = 0; i < n; i++) {
        triangle[i] = (int *)malloc((i + 1) * sizeof(int));
        if (triangle[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(triangle[j]);
            }
            free(triangle);
            return 1;
        }
    }
    int row0[] = {1};
    int row1[] = {2, 3};
    int row2[] = {6, 5, 7};
    int row3[] = {4, 1, 8, 3};
    triangle[0] = row0;
    triangle[1] = row1;
    triangle[2] = row2;
    triangle[3] = row3;
    int result = find_max_sum(triangle, n);
    printf("%d\n", result);
    for (int i = 0; i < n; i++) {
        free(triangle[i]);
    }
    free(triangle);
    return 0;
}