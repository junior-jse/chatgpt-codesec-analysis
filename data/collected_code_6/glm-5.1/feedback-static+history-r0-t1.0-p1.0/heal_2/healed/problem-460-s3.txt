#include <stdio.h>
#include <stdlib.h>

int *get_first_elements(int **lists, const int *sizes, int num_lists) {
    if (lists == NULL || sizes == NULL || num_lists <= 0) {
        return NULL;
    }

    int *result = malloc(num_lists * sizeof(int));
    if (result == NULL) {
        return NULL;
    }

    for (int i = 0; i < num_lists; i++) {
        if (lists[i] == NULL || sizes[i] <= 0) {
            free(result);
            return NULL;
        }
        result[i] = lists[i][0];
    }

    return result;
}

int main(void) {
    int list1[] = {1, 2, 3};
    int list2[] = {4, 5, 6};
    int list3[] = {7, 8, 9};

    int *lists[] = {list1, list2, list3};
    const int sizes[] = {3, 3, 3};
    int num_lists = 3;

    int *first_elements = get_first_elements(lists, sizes, num_lists);
    if (first_elements != NULL) {
        for (int i = 0; i < num_lists; i++) {
            printf("%d\n", first_elements[i]);
        }
        free(first_elements);
    }

    return 0;
}