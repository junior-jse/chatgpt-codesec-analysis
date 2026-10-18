#include <stdio.h>
#include <stdlib.h>

int* remove_two_duplicates(int** lists, int* list_sizes, int num_lists, int** out_sizes) {
    int total_max_elements = 0;
    for (int i = 0; i < num_lists; i++) {
        total_max_elements += list_sizes[i];
    }

    int* result = malloc(total_max_elements * sizeof(int));
    if (!result) {
        *out_sizes = NULL;
        return NULL;
    }

    *out_sizes = malloc(num_lists * sizeof(int));
    if (!*out_sizes) {
        free(result);
        return NULL;
    }

    int pos = 0;
    for (int i = 0; i < num_lists; i++) {
        int* list = lists[i];
        int size = list_sizes[i];
        int out_idx = 0;

        int* temp = malloc(size * sizeof(int));
        if (!temp) {
            for (int j = 0; j < pos; j++) {}
            free(result);
            free(*out_sizes);
            *out_sizes = NULL;
            return NULL;
        }

        int j = 0;
        while (j < size) {
            int count = 1;
            while (j + count < size && list[j + count] == list[j] && count < 2) {
                count++;
            }

            if (count >= 2) {
                temp[out_idx++] = list[j];
                j += count;
            } else {
                temp[out_idx++] = list[j];
                j++;
            }
        }

        for (int k = 0; k < out_idx; k++) {
            result[pos++] = temp[k];
        }
        (*out_sizes)[i] = out_idx;
        free(temp);
    }

    return result;
}

int main(void) {
    int list1[] = {1, 2, 2, 3, 4, 4, 4, 5};
    int list2[] = {6, 7, 7, 7, 8};
    int list3[] = {9, 9, 10, 10, 10, 11};
    
    int* lists[] = {list1, list2, list3};
    int list_sizes[] = {8, 5, 6};
    int num_lists = 3;
    int* out_sizes = NULL;

    int* result = remove_two_duplicates(lists, list_sizes, num_lists, &out_sizes);

    if (result && out_sizes) {
        int offset = 0;
        for (int i = 0; i < num_lists; i++) {
            for (int j = 0; j < out_sizes[i]; j++) {
                printf("%d ", result[offset + j]);
            }
            printf("\n");
            offset += out_sizes[i];
        }
        free(result);
        free(out_sizes);
    }

    return 0;
}