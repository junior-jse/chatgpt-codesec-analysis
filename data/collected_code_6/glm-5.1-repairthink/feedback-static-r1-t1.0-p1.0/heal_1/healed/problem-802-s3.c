#include <stdio.h>
#include <stdlib.h>

int count_rotations(const int *arr, int n) {
    if (arr == NULL || n <= 0) {
        return -1;
    }
    if (n == 1) {
        return 0;
    }

    int low = 0;
    int high = n - 1;

    while (low <= high) {
        if (arr[low] <= arr[high]) {
            return low;
        }

        int mid = low + (high - low) / 2;
        int next = (mid + 1) % n;
        int prev = (mid - 1 + n) % n;

        if (arr[mid] <= arr[next] && arr[mid] <= arr[prev]) {
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
    int n1 = sizeof(arr1) / sizeof(arr1[0]);
    int res1 = count_rotations(arr1, n1);
    printf("%d\n", res1);

    int arr2[] = {7, 9, 11, 12, 5};
    int n2 = sizeof(arr2) / sizeof(arr2[0]);
    int res2 = count_rotations(arr2, n2);
    printf("%d\n", res2);

    int arr3[] = {1, 2, 3, 4, 5};
    int n3 = sizeof(arr3) / sizeof(arr3[0]);
    int res3 = count_rotations(arr3, n3);
    printf("%d\n", res3);

    int res4 = count_rotations(NULL, 5);
    printf("%d\n", res4);

    return 0;
}