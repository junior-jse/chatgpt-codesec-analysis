#include <stdio.h>
#include <stdlib.h>

int count_rotations(const int *arr, int size) {
    if (arr == NULL || size <= 0) {
        return -1;
    }

    int left = 0;
    int right = size - 1;

    while (left <= right) {
        if (arr[left] <= arr[right]) {
            return left;
        }

        int mid = left + (right - left) / 2;
        int next = (mid + 1) % size;
        int prev = (mid + size - 1) % size;

        if (arr[mid] <= arr[next] && arr[mid] <= arr[prev]) {
            return mid;
        }
        else if (arr[mid] <= arr[right]) {
            right = mid - 1;
        }
        else if (arr[mid] >= arr[left]) {
            left = mid + 1;
        }
    }

    return -1;
}

int main(void) {
    int arr1[] = {15, 18, 2, 3, 6, 12};
    int size1 = sizeof(arr1) / sizeof(arr1[0]);
    int rotations1 = count_rotations(arr1, size1);
    printf("Rotations for array 1: %d\n", rotations1);

    int arr2[] = {7, 9, 11, 12, 5};
    int size2 = sizeof(arr2) / sizeof(arr2[0]);
    int rotations2 = count_rotations(arr2, size2);
    printf("Rotations for array 2: %d\n", rotations2);

    int arr3[] = {1, 2, 3, 4, 5};
    int size3 = sizeof(arr3) / sizeof(arr3[0]);
    int rotations3 = count_rotations(arr3, size3);
    printf("Rotations for array 3: %d\n", rotations3);

    int arr4[] = {2, 1};
    int size4 = sizeof(arr4) / sizeof(arr4[0]);
    int rotations4 = count_rotations(arr4, size4);
    printf("Rotations for array 4: %d\n", rotations4);

    int rotations5 = count_rotations(NULL, 5);
    printf("Rotations for NULL array: %d\n", rotations5);

    return 0;
}