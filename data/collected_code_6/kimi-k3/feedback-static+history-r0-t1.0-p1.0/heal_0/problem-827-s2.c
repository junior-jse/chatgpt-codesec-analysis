#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

long sum_column(int **lists, size_t num_lists, size_t column_index, size_t *list_lengths) {
    if (lists == NULL || list_lengths == NULL) {
        return 0;
    }

    long sum = 0;
    for (size_t i = 0; i < num_lists; i++) {
        if (lists[i] == NULL) {
            continue;
        }
        if (column_index < list_lengths[i]) {
            sum += lists[i][column_index];
        }
    }
    return sum;
}

int main(void) {
    size_t num_lists = 3;
    size_t column_index = 1;
    
    size_t *list_lengths = malloc(num_lists * sizeof(size_t));
    if (list_lengths == NULL) {
        return EXIT_FAILURE;
    }
    
    int **lists = malloc(num_lists * sizeof(int *));
    if (lists == NULL) {
        free(list_lengths);
        return EXIT_FAILURE;
    }
    
    list_lengths[0] = 3;
    lists[0] = malloc(list_lengths[0] * sizeof(int));
    if (lists[0] == NULL) {
        free(lists);
        free(list_lengths);
        return EXIT_FAILURE;
    }
    lists[0][0] = 1;
    lists[0][1] = 2;
    lists[0][2] = 3;
    
    list_lengths[1] = 2;
    lists[1] = malloc(list_lengths[1] * sizeof(int));
    if (lists[1] == NULL) {
        free(lists[0]);
        free(lists);
        free(list_lengths);
        return EXIT_FAILURE;
    }
    lists[1][0] = 4;
    lists[1][1] = 5;
    
    list_lengths[2] = 4;
    lists[2] = malloc(list_lengths[2] * sizeof(int));
    if (lists[2] == NULL) {
        free(lists[1]);
        free(lists[0]);
        free(lists);
        free(list_lengths);
        return EXIT_FAILURE;
    }
    lists[2][0] = 6;
    lists[2][1] = 7;
    lists[2][2] = 8;
    lists[2][3] = 9;
    
    long result = sum_column(lists, num_lists, column_index, list_lengths);
    printf("Sum of column %zu: %ld\n", column_index, result);
    
    for (size_t i = 0; i < num_lists; i++) {
        free(lists[i]);
    }
    free(lists);
    free(list_lengths);
    
    return EXIT_SUCCESS;
}