#include <stdlib.h>
#include <stdio.h>

int** allocate_matrix(int rows, int cols) {
    int **matrix = (int **)malloc(rows * sizeof(int *));
    if (!matrix) return NULL;
    for (int i = 0; i < rows; i++) {
        matrix[i] = (int *)malloc(cols * sizeof(int));
        if (!matrix[i]) {
            for (int j = 0; j < i; j++) free(matrix[j]);
            free(matrix);
            return NULL;
        }
    }
    return matrix;
}

void free_matrix(int **matrix, int rows) {
    if (!matrix) return;
    for (int i = 0; i < rows; i++) free(matrix[i]);
    free(matrix);
}

int*** zip_lists(int ***list1, int **sizes1, int len1, int ***list2, int **sizes2, int len2) {
    if (len1 <= 0 || len2 <= 0) return NULL;

    int min_len = len1 < len2 ? len1 : len2;
    int ***result = (int ***)malloc(min_len * sizeof(int **));
    if (!result) return NULL;

    for (int i = 0; i < min_len; i++) {
        int inner_len1 = sizes1[i][0];
        int inner_len2 = sizes2[i][0];
        int total_len = inner_len1 + inner_len2;

        result[i] = allocate_matrix(1, total_len);
        if (!result[i]) {
            for (int j = 0; j < i; j++) {
                free_matrix(result[j], 1);
            }
            free(result);
            return NULL;
        }

        for (int j = 0; j < inner_len1; j++) {
            result[i][0][j] = list1[i][0][j];
        }
        for (int j = 0; j < inner_len2; j++) {
            result[i][0][inner_len1 + j] = list2[i][0][j];
        }
    }

    return result;
}

int main(void) {
    int sizes1_0[] = {2};
    int list1_0[] = {1, 2};
    int *row1_0[] = {list1_0};

    int sizes1_1[] = {1};
    int list1_1[] = {5};
    int *row1_1[] = {list1_1};

    int **list1[] = {row1_0, row1_1};
    int *sizes1[] = {sizes1_0, sizes1_1};

    int sizes2_0[] = {3};
    int list2_0[] = {3, 4, 5};
    int *row2_0[] = {list2_0};

    int sizes2_1[] = {2};
    int list2_1[] = {6, 7};
    int *row2_1[] = {list2_1};

    int **list2[] = {row2_0, row2_1};
    int *sizes2[] = {sizes2_0, sizes2_1};

    int ***zipped = zip_lists(list1, sizes1, 2, list2, sizes2, 2);

    if (zipped) {
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < sizes1[i][0] + sizes2[i][0]; j++) {
                printf("%d ", zipped[i][0][j]);
            }
            printf("\n");
            free_matrix(zipped[i], 1);
        }
        free(zipped);
    }

    return 0;
}