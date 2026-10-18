#include <stdlib.h>
#include <stdio.h>

int **interleave(int **lists, int list_count, int list_length) {
    if (lists == NULL || list_count == 0 || list_length == 0) {
        return NULL;
    }

    for (int i = 0; i < list_count; i++) {
        if (lists[i] == NULL) {
            return NULL;
        }
    }

    int **result = (int **)malloc(list_count * sizeof(int *));
    if (result == NULL) {
        return NULL;
    }

    for (int i = 0; i < list_count; i++) {
        result[i] = (int *)malloc(list_length * sizeof(int));
        if (result[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
    }

    for (int pos = 0; pos < list_length; pos++) {
        for (int list = 0; list < list_count; list++) {
            result[list][pos] = lists[list][pos];
        }
    }

    int *interleaved = (int *)malloc(list_count * list_length * sizeof(int));
    if (interleaved == NULL) {
        for (int i = 0; i < list_count; i++) {
            free(result[i]);
        }
        free(result);
        return NULL;
    }

    int idx = 0;
    for (int pos = 0; pos < list_length; pos++) {
        for (int list = 0; list < list_count; list++) {
            interleaved[idx++] = result[list][pos];
        }
    }

    for (int i = 0; i < list_count; i++) {
        free(result[i]);
    }
    free(result);

    int **final_result = (int **)malloc(sizeof(int *));
    if (final_result == NULL) {
        free(interleaved);
        return NULL;
    }
    final_result[0] = interleaved;

    return final_result;
}

int main(void) {
    int list1[] = {1, 2, 3};
    int list2[] = {4, 5, 6};
    int list3[] = {7, 8, 9};

    int *lists[] = {list1, list2, list3};

    int **result = interleave(lists, 3, 3);
    if (result == NULL) {
        return 1;
    }

    for (int i = 0; i < 9; i++) {
        printf("%d ", result[0][i]);
    }
    printf("\n");

    free(result[0]);
    free(result);

    return 0;
}