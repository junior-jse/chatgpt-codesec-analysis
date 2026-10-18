#include <stdio.h>
#include <stdlib.h>

void interchange_first_last(int *arr, int size) {
    int temp;
    if (size <= 1) {
        return;
    }
    temp = arr[0];
    arr[0] = arr[size - 1];
    arr[size - 1] = temp;
}

int main(void) {
    int arr[] = {1, 2, 3, 4, 5};
    int size = sizeof(arr) / sizeof(arr[0]);
    int i;

    printf("Before: ");
    for (i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    interchange_first_last(arr, size);

    printf("After:  ");
    for (i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    return 0;
}