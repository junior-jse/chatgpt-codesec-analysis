#include <stdio.h>
#include <stdlib.h>

int* remove_two_duplicates(int** lists, int* list_sizes, int num_lists, int* result_size) {
    int total_elements = 0;
    for (int i = 0; i < num_lists; i++) {
        total_elements += list_sizes[i];
    }

    if (total_elements == 0) {
        *result_size = 0;
        return NULL;
    }

    int* all_elements = (int*)malloc(total_elements * sizeof(int));
    if (!all_elements) {
        *result_size = 0;
        return NULL;
    }

    int index = 0;
    for (int i = 0; i < num_lists; i++) {
        for (int j = 0; j < list_sizes[i]; j++) {
            all_elements[index++] = lists[i][j];
        }
    }

    for (int i = 0; i < total_elements; i++) {
        for (int j = i + 1; j < total_elements; j++) {
            if (all_elements[i] > all_elements[j]) {
                int temp = all_elements[i];
                all_elements[i] = all_elements[j];
                all_elements[j] = temp;
            }
        }
    }

    int* temp_result = (int*)malloc(total_elements * sizeof(int));
    if (!temp_result) {
        free(all_elements);
        *result_size = 0;
        return NULL;
    }

    int count = 0;
    int i = 0;
    while (i < total_elements) {
        if (i + 1 < total_elements && all_elements[i] == all_elements[i + 1]) {
            i += 2;
        } else {
            temp_result[count++] = all_elements[i];
            i++;
        }
    }

    free(all_elements);

    int* final_result = (int*)malloc(count * sizeof(int));
    if (!final_result) {
        free(temp_result);
        *result_size = 0;
        return NULL;
    }

    for (int j = 0; j < count; j++) {
        final_result[j] = temp_result[j];
    }

    free(temp_result);

    *result_size = count;
    return final_result;
}

int main() {
    int list1[] = {1, 2, 3, 4};
    int list2[] = {3, 4, 5, 6};
    int list3[] = {5, 6, 7, 8};

    int* lists[] = {list1, list2, list3};
    int list_sizes[] = {4, 4, 4};
    int num_lists = 3;
    int result_size = 0;

    int* result = remove_two_duplicates(lists, list_sizes, num_lists, &result_size);

    if (result) {
        for (int i = 0; i < result_size; i++) {
            printf("%d ", result[i]);
        }
        printf("\n");
        free(result);
    } else {
        printf("No elements or allocation failed.\n");
    }

    return 0;
}