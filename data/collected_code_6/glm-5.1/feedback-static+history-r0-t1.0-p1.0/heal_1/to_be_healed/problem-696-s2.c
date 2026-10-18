#include <stdio.h>
#include <stdlib.h>

int** zip_lists_of_lists(int** list1, int** list2, int outer_size, const int* inner_sizes, int** out_inner_sizes) {
    int** result = (int**)malloc(outer_size * sizeof(int*));
    if (result == NULL) {
        return NULL;
    }

    *out_inner_sizes = (int*)malloc(outer_size * sizeof(int));
    if (*out_inner_sizes == NULL) {
        free(result);
        return NULL;
    }

    for (int i = 0; i < outer_size; i++) {
        int current_inner_size = inner_sizes[i] * 2;
        (*out_inner_sizes)[i] = current_inner_size;
        
        result[i] = (int*)malloc(current_inner_size * sizeof(int));
        if (result[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(result[j]);
            }
            free(*out_inner_sizes);
            *out_inner_sizes = NULL;
            free(result);
            return NULL;
        }

        for (int k = 0; k < inner_sizes[i]; k++) {
            result[i][k * 2] = list1[i][k];
            result[i][k * 2 + 1] = list2[i][k];
        }
    }

    return result;
}

void free_zipped_lists(int** lists, int* inner_sizes, int outer_size) {
    if (lists == NULL) {
        return;
    }
    for (int i = 0; i < outer_size; i++) {
        free(lists[i]);
    }
    free(lists);
    free(inner_sizes);
}

int main() {
    int row1[] = {1, 2, 3};
    int row2[] = {4, 5, 6};
    int row3[] = {7, 8, 9};
    int row4[] = {10, 11, 12};
    int row5[] = {13, 14, 15};
    int row6[] = {16, 17, 18};

    int* list1[] = {row1, row2, row3};
    int* list2[] = {row4, row5, row6};
    
    int outer_size = 3;
    /* Possible weaknesses found:
     *  Variable 'inner_sizes' can be declared as const array [constVariable]
     */
    int inner_sizes[] = {3, 3, 3};
    int* out_inner_sizes = NULL;

    int** zipped = zip_lists_of_lists(list1, list2, outer_size, inner_sizes, &out_inner_sizes);

    if (zipped == NULL) {
        return 1;
    }

    for (int i = 0; i < outer_size; i++) {
        for (int j = 0; j < out_inner_sizes[i]; j++) {
            printf("%d ", zipped[i][j]);
        }
        printf("\n");
    }

    free_zipped_lists(zipped, out_inner_sizes, outer_size);

    return 0;
}