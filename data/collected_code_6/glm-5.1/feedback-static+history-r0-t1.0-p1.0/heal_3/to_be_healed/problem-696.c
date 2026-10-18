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

int** zip_lists(int **list1, const int *sizes1, int len1, int **list2, const int *sizes2, int len2) {
    if (len1 <= 0 || len2 <= 0) return NULL;

    int min_len = len1 < len2 ? len1 : len2;
    int **result = (int **)malloc(min_len * sizeof(int *));
    if (!result) return NULL;

    int *result_sizes = (int *)malloc(min_len * sizeof(int));
    if (!result_sizes) {
        free(result);
        return NULL;
    }

    for (int i = 0; i < min_len; i++) {
        int inner_len1 = sizes1[i];
        int inner_len2 = sizes2[i];
        int total_len = inner_len1 + inner_len2;

        result[i] = (int *)malloc(total_len * sizeof(int));
        if (!result[i]) {
            for (int j = 0; j < i; j++) free(result[j]);
            free(result);
            free(result_sizes);
            return NULL;
        }

        result_sizes[i] = total_len;

        for (int j = 0; j < inner_len1; j++) {
            result[i][j] = list1[i][j];
        }
        for (int j = 0; j < inner_len2; j++) {
            result[i][inner_len1 + j] = list2[i][j];
        }
    }

    free(result_sizes);
    return result;
}

int main(void) {
    int list1_0[] = {1, 2};
    int list1_1[] = {5};
    int *list1[] = {list1_0, list1_1};
    /* Possible weaknesses found:
     *  Variable 'sizes1' can be declared as const array [constVariable]
     */
    int sizes1[] = {2, 1};

    int list2_0[] = {3, 4, 5};
    int list2_1[] = {6, 7};
    int *list2[] = {list2_0, list2_1};
    /* Possible weaknesses found:
     *  Variable 'sizes2' can be declared as const array [constVariable]
     */
    int sizes2[] = {3, 2};

    int **zipped = zip_lists(list1, sizes1, 2, list2, sizes2, 2);

    if (zipped) {
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < sizes1[i] + sizes2[i]; j++) {
                printf("%d ", zipped[i][j]);
            }
            printf("\n");
            free(zipped[i]);
        }
        free(zipped);
    }

    return 0;
}