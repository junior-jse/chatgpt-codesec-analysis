#include <stdio.h>
#include <stdlib.h>

int remove_two_duplicates(int *arr, int size, int *new_size) {
    if (arr == NULL || new_size == NULL || size < 0) {
        return -1;
    }
    if (size == 0) {
        *new_size = 0;
        return 0;
    }
    int *temp = (int *)malloc((size_t)size * sizeof(int));
    if (temp == NULL) {
        return -1;
    }
    int temp_count = 0;
    for (int i = 0; i < size; i++) {
        int occurrences = 0;
        for (int j = 0; j < size; j++) {
            if (arr[j] == arr[i]) {
                occurrences++;
            }
        }
        if (occurrences == 2) {
            continue;
        }
        int already_added = 0;
        for (int k = 0; k < temp_count; k++) {
            if (temp[k] == arr[i]) {
                already_added = 1;
                break;
            }
        }
        if (!already_added) {
            temp[temp_count++] = arr[i];
        }
    }
    for (int i = 0; i < temp_count; i++) {
        arr[i] = temp[i];
    }
    *new_size = temp_count;
    free(temp);
    temp = NULL;
    return 0;
}

int main(void) {
    int arr[] = {1, 2, 2, 3, 4, 4, 5, 6, 6, 6, 7};
    int size = (int)(sizeof(arr) / sizeof(arr[0]));
    int new_size = 0;
    if (remove_two_duplicates(arr, size, &new_size) != 0) {
        fprintf(stderr, "Error processing array\n");
        return EXIT_FAILURE;
    }
    printf("Result: ");
    for (int i = 0; i < new_size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    return EXIT_SUCCESS;
}