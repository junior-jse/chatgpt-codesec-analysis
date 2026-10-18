#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int **elements;
    size_t rows;
    size_t *cols;
} NestedList;

int *extract_column(const NestedList *list, size_t column, size_t *out_len) {
    if (list == NULL || out_len == NULL) {
        return NULL;
    }

    *out_len = 0;
    size_t valid_rows = 0;

    for (size_t i = 0; i < list->rows; i++) {
        if (column < list->cols[i]) {
            valid_rows++;
        }
    }

    if (valid_rows == 0) {
        return NULL;
    }

    int *result = (int *)malloc(valid_rows * sizeof(int));
    if (result == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < list->rows; i++) {
        if (column < list->cols[i]) {
            result[*out_len] = list->elements[i][column];
            (*out_len)++;
        }
    }

    return result;
}

void free_nested_list(NestedList *list) {
    if (list == NULL) {
        return;
    }

    for (size_t i = 0; i < list->rows; i++) {
        free(list->elements[i]);
    }

    free(list->elements);
    free(list->cols);

    list->elements = NULL;
    list->cols = NULL;
    list->rows = 0;
}

int main(void) {
    NestedList list;
    list.rows = 3;

    list.cols = (size_t *)malloc(list.rows * sizeof(size_t));
    if (list.cols == NULL) {
        return EXIT_FAILURE;
    }

    list.cols[0] = 4;
    list.cols[1] = 2;
    list.cols[2] = 5;

    list.elements = (int **)malloc(list.rows * sizeof(int *));
    if (list.elements == NULL) {
        free(list.cols);
        return EXIT_FAILURE;
    }

    list.elements[0] = (int *)malloc(4 * sizeof(int));
    list.elements[1] = (int *)malloc(2 * sizeof(int));
    list.elements[2] = (int *)malloc(5 * sizeof(int));

    if (list.elements[0] == NULL || list.elements[1] == NULL || list.elements[2] == NULL) {
        free(list.elements[0]);
        free(list.elements[1]);
        free(list.elements[2]);
        free(list.elements);
        free(list.cols);
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < 4; i++) list.elements[0][i] = (int)(i + 1);
    for (size_t i = 0; i < 2; i++) list.elements[1][i] = (int)(i + 10);
    for (size_t i = 0; i < 5; i++) list.elements[2][i] = (int)(i + 20);

    size_t out_len;
    int *column_1 = extract_column(&list, 1, &out_len);

    if (column_1 != NULL) {
        for (size_t i = 0; i < out_len; i++) {
            printf("%d ", column_1[i]);
        }
        printf("\n");
        free(column_1);
    }

    int *column_3 = extract_column(&list, 3, &out_len);

    if (column_3 != NULL) {
        for (size_t i = 0; i < out_len; i++) {
            printf("%d ", column_3[i]);
        }
        printf("\n");
        free(column_3);
    }

    free_nested_list(&list);

    return EXIT_SUCCESS;
}