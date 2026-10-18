#include <stdio.h>
#include <stdlib.h>

int remove_duplicates(int **lists, int *sizes, int num_lists) {
    if (lists == NULL || sizes == NULL || num_lists <= 0) {
        return -1;
    }

    for (int i = 0; i < num_lists; i++) {
        if (lists[i] == NULL || sizes[i] < 0) {
            return -1;
        }

        int found_duplicate = 0;
        int value_to_remove = 0;

        for (int j = 0; j < sizes[i] && !found_duplicate; j++) {
            int count = 0;
            for (int k = 0; k < sizes[i]; k++) {
                if (lists[i][j] == lists[i][k]) {
                    count++;
                }
            }

            if (count > 1) {
                value_to_remove = lists[i][j];
                found_duplicate = 1;
            }
        }

        if (found_duplicate) {
            int write_idx = 0;
            int removed = 0;

            for (int k = 0; k < sizes[i]; k++) {
                if (lists[i][k] != value_to_remove || removed >= 2) {
                    lists[i][write_idx++] = lists[i][k];
                } else {
                    removed++;
                }
            }

            sizes[i] = write_idx;
            int *new_list = realloc(lists[i], sizes[i] * sizeof(int));
            if (new_list == NULL && sizes[i] > 0) {
                return -1;
            }
            lists[i] = new_list;
        }
    }

    return 0;
}

int main(void) {
    int num_lists = 3;
    int **lists = malloc(num_lists * sizeof(int *));
    int *sizes = malloc(num_lists * sizeof(int));

    if (lists == NULL || sizes == NULL) {
        free(lists);
        free(sizes);
        return 1;
    }

    sizes[0] = 6;
    lists[0] = malloc(sizes[0] * sizeof(int));
    if (lists[0] == NULL) {
        free(lists);
        free(sizes);
        return 1;
    }
    const int temp0[] = {1, 2, 2, 3, 4, 2};
    for (int i = 0; i < sizes[0]; i++) lists[0][i] = temp0[i];

    sizes[1] = 5;
    lists[1] = malloc(sizes[1] * sizeof(int));
    if (lists[1] == NULL) {
        for (int i = 0; i < 1; i++) free(lists[i]);
        free(lists);
        free(sizes);
        return 1;
    }
    const int temp1[] = {5, 6, 7, 7, 8};
    for (int i = 0; i < sizes[1]; i++) lists[1][i] = temp1[i];

    sizes[2] = 4;
    lists[2] = malloc(sizes[2] * sizeof(int));
    if (lists[2] == NULL) {
        for (int i = 0; i < 2; i++) free(lists[i]);
        free(lists);
        free(sizes);
        return 1;
    }
    const int temp2[] = {9, 10, 10, 11};
    for (int i = 0; i < sizes[2]; i++) lists[2][i] = temp2[i];

    int result = remove_duplicates(lists, sizes, num_lists);

    if (result == 0) {
        for (int i = 0; i < num_lists; i++) {
            printf("List %d: ", i);
            for (int j = 0; j < sizes[i]; j++) {
                printf("%d ", lists[i][j]);
            }
            printf("\n");
        }
    } else {
        printf("Error occurred\n");
    }

    for (int i = 0; i < num_lists; i++) {
        free(lists[i]);
    }
    free(lists);
    free(sizes);

    return 0;
}