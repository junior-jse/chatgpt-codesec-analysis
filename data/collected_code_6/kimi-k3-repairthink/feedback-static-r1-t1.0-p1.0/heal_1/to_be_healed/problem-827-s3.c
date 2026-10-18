#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

/* Possible weaknesses found:
 *  Parameter 'list_sizes' can be declared as pointer to const [constParameterPointer]
 */
int sum_column(int **lists, int num_lists, int *list_sizes, int column_index, long *result) {
    int i;
    long sum;

    if (lists == NULL || list_sizes == NULL || result == NULL) {
        return -1;
    }

    if (num_lists <= 0 || column_index < 0) {
        return -1;
    }

    sum = 0;

    for (i = 0; i < num_lists; i++) {
        if (lists[i] == NULL) {
            return -1;
        }

        if (column_index >= list_sizes[i]) {
            return -1;
        }

        sum += lists[i][column_index];
    }

    *result = sum;
    return 0;
}

int main(void) {
    int **lists = NULL;
    int *list_sizes = NULL;
    int num_lists = 3;
    int column_index = 1;
    long result = 0;
    int i;
    int status;

    list_sizes = malloc(num_lists * sizeof(int));
    if (list_sizes == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    lists = malloc(num_lists * sizeof(int *));
    if (lists == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(list_sizes);
        return EXIT_FAILURE;
    }

    list_sizes[0] = 3;
    list_sizes[1] = 4;
    list_sizes[2] = 3;

    lists[0] = malloc(list_sizes[0] * sizeof(int));
    if (lists[0] == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(lists);
        free(list_sizes);
        return EXIT_FAILURE;
    }

    lists[1] = malloc(list_sizes[1] * sizeof(int));
    if (lists[1] == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(lists[0]);
        free(lists);
        free(list_sizes);
        return EXIT_FAILURE;
    }

    lists[2] = malloc(list_sizes[2] * sizeof(int));
    if (lists[2] == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(lists[1]);
        free(lists[0]);
        free(lists);
        free(list_sizes);
        return EXIT_FAILURE;
    }

    lists[0][0] = 1;
    lists[0][1] = 2;
    lists[0][2] = 3;

    lists[1][0] = 4;
    lists[1][1] = 5;
    lists[1][2] = 6;
    lists[1][3] = 7;

    lists[2][0] = 8;
    lists[2][1] = 9;
    lists[2][2] = 10;

    status = sum_column(lists, num_lists, list_sizes, column_index, &result);

    if (status == 0) {
        printf("Sum of column %d: %ld\n", column_index, result);
    } else {
        fprintf(stderr, "Error: Invalid parameters or column index out of bounds\n");
    }

    for (i = 0; i < num_lists; i++) {
        free(lists[i]);
    }
    free(lists);
    free(list_sizes);

    return (status == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}