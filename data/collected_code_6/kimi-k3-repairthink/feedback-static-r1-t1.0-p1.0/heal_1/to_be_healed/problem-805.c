#include <stdio.h>
#include <stdlib.h>

/* Possible weaknesses found:
 *  Parameter 'sizes' can be declared as pointer to const [constParameterPointer]
 */
int* find_max_sum_list(int** lists, int num_lists, int* sizes, int* max_size) {
    if (lists == NULL || num_lists <= 0 || sizes == NULL || max_size == NULL) {
        return NULL;
    }

    int max_sum = 0;
    int max_index = -1;

    for (int i = 0; i < num_lists; i++) {
        if (lists[i] == NULL || sizes[i] <= 0) {
            continue;
        }

        int current_sum = 0;
        for (int j = 0; j < sizes[i]; j++) {
            current_sum += lists[i][j];
        }

        if (max_index == -1 || current_sum > max_sum) {
            max_sum = current_sum;
            max_index = i;
            *max_size = sizes[i];
        }
    }

    if (max_index == -1) {
        return NULL;
    }

    return lists[max_index];
}

int main(void) {
    int* list1 = (int*)malloc(3 * sizeof(int));
    int* list2 = (int*)malloc(4 * sizeof(int));
    int* list3 = (int*)malloc(2 * sizeof(int));

    if (list1 == NULL || list2 == NULL || list3 == NULL) {
        free(list1);
        free(list2);
        free(list3);
        return EXIT_FAILURE;
    }

    list1[0] = 1; list1[1] = 2; list1[2] = 3;
    list2[0] = 10; list2[1] = 20; list2[2] = 30; list2[3] = 40;
    list3[0] = 5; list3[1] = 5;

    int** lists = (int**)malloc(3 * sizeof(int*));
    int* sizes = (int*)malloc(3 * sizeof(int));

    if (lists == NULL || sizes == NULL) {
        free(list1);
        free(list2);
        free(list3);
        free(lists);
        free(sizes);
        return EXIT_FAILURE;
    }

    lists[0] = list1;
    lists[1] = list2;
    lists[2] = list3;

    sizes[0] = 3;
    sizes[1] = 4;
    sizes[2] = 2;

    int max_size = 0;
    /* Possible weaknesses found:
     *  Variable 'result' can be declared as pointer to const [constVariablePointer]
     */
    int* result = find_max_sum_list(lists, 3, sizes, &max_size);

    if (result != NULL) {
        printf("List with highest sum: ");
        for (int i = 0; i < max_size; i++) {
            printf("%d ", result[i]);
        }
        printf("\n");
    } else {
        printf("No valid list found.\n");
    }

    free(list1);
    free(list2);
    free(list3);
    free(lists);
    free(sizes);

    return EXIT_SUCCESS;
}