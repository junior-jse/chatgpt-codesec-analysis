#include <stdio.h>
#include <stdlib.h>

size_t count_rotations(int *arr, size_t n) {
    if (arr == NULL) {
        return 0;
    }

    if (n == 0 || n == 1) {
        return 0;
    }

    size_t low = 0;
    size_t high = n - 1;

    while (low <= high) {
        if (arr[low] <= arr[high]) {
            return low;
        }

        size_t mid = low + (high - low) / 2;
        size_t prev = (mid - 1 + n) % n;
        size_t next = (mid + 1) % n;

        if (arr[mid] <= arr[prev] && arr[mid] <= arr[next]) {
            return mid;
        }

        if (arr[mid] <= arr[high]) {
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }

    return 0;
}

int main(void) {
    int arr1[] = {15, 18, 2, 3, 6, 12};
    size_t n1 = sizeof(arr1) / sizeof(arr1[0]);
    size_t rotations1 = count_rotations(arr1, n1);
    printf("%zu\n", rotations1);

    int arr2[] = {7, 9, 11, 12, 5};
    size_t n2 = sizeof(arr2) / sizeof(arr2[0]);
    size_t rotations2 = count_rotations(arr2, n2);
    printf("%zu\n", rotations2);

    int arr3[] = {1, 2, 3, 4, 5};
    size_t n3 = sizeof(arr3) / sizeof(arr3[0]);
    size_t rotations3 = count_rotations(arr3, n3);
    printf("%zu\n", rotations3);

    size_t rotations4 = count_rotations(NULL, n3);
    printf("%zu\n", rotations4);

    size_t rotations5 = count_rotations(arr3, 0);
    printf("%zu\n", rotations5);

    return 0;
}