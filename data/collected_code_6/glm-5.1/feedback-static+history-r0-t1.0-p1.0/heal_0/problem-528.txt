#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int **lists;
    int *lengths;
    int count;
} ListOfLists;

ListOfLists find_min_length_lists(ListOfLists input) {
    ListOfLists result;
    result.lists = NULL;
    result.lengths = NULL;
    result.count = 0;

    if (input.count <= 0 || input.lists == NULL || input.lengths == NULL) {
        return result;
    }

    int min_len = input.lengths[0];
    for (int i = 1; i < input.count; i++) {
        if (input.lengths[i] < min_len) {
            min_len = input.lengths[i];
        }
    }

    int min_count = 0;
    for (int i = 0; i < input.count; i++) {
        if (input.lengths[i] == min_len) {
            min_count++;
        }
    }

    if (min_count == 0) {
        return result;
    }

    result.lists = (int **)malloc(min_count * sizeof(int *));
    result.lengths = (int *)malloc(min_count * sizeof(int));
    
    if (result.lists == NULL || result.lengths == NULL) {
        free(result.lists);
        free(result.lengths);
        result.lists = NULL;
        result.lengths = NULL;
        result.count = 0;
        return result;
    }

    int idx = 0;
    for (int i = 0; i < input.count; i++) {
        if (input.lengths[i] == min_len) {
            result.lists[idx] = input.lists[i];
            result.lengths[idx] = input.lengths[i];
            idx++;
        }
    }

    result.count = min_count;
    return result;
}

int main() {
    int a[] = {1, 2};
    int b[] = {3};
    int c[] = {4, 5, 6};
    int d[] = {7};

    int *lists[] = {a, b, c, d};
    int lengths[] = {2, 1, 3, 1};
    
    ListOfLists input;
    input.lists = lists;
    input.lengths = lengths;
    input.count = 4;

    ListOfLists result = find_min_length_lists(input);

    for (int i = 0; i < result.count; i++) {
        printf("List %d (length %d): ", i, result.lengths[i]);
        for (int j = 0; j < result.lengths[i]; j++) {
            printf("%d ", result.lists[i][j]);
        }
        printf("\n");
    }

    free(result.lists);
    free(result.lengths);

    return 0;
}