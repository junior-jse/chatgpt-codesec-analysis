#include <stdio.h>
#include <stdlib.h>

void rotate_left(int *list, size_t size, size_t items) {
    if (size == 0 || items == 0) {
        return;
    }

    size_t offset = items % size;
    if (offset == 0) {
        return;
    }

    int *temp = (int *)malloc(offset * sizeof(int));
    if (temp == NULL) {
        return;
    }

    for (size_t i = 0; i < offset; i++) {
        temp[i] = list[i];
    }

    for (size_t i = 0; i < size - offset; i++) {
        list[i] = list[i + offset];
    }

    for (size_t i = 0; i < offset; i++) {
        list[size - offset + i] = temp[i];
    }

    free(temp);
}

int main(void) {
    int arr[] = {1, 2, 3, 4, 5, 6, 7};
    size_t size = sizeof(arr) / sizeof(arr[0]);
    size_t items = 3;

    for (size_t i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    rotate_left(arr, size, items);

    for (size_t i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    return 0;
}