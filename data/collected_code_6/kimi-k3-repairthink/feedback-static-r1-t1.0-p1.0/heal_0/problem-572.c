#include <stdio.h>
#include <stdlib.h>

void removeDuplicates(int *arr, int *size) {
    int i, j, k;
    for (i = 0; i < *size; i++) {
        for (j = i + 1; j < *size; j++) {
            if (arr[i] == arr[j]) {
                for (k = j; k < *size - 1; k++) {
                    arr[k] = arr[k + 1];
                }
                (*size)--;
                j--;
            }
        }
    }
}

int main() {
    int *arr;
    int size, i;

    printf("Enter the number of elements: ");
    scanf("%d", &size);

    arr = (int *)malloc(size * sizeof(int));
    if (arr == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    printf("Enter the elements: ");
    for (i = 0; i < size; i++) {
        scanf("%d", &arr[i]);
    }

    removeDuplicates(arr, &size);

    printf("Array after removing duplicates: ");
    for (i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    free(arr);
    return 0;
}