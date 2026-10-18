#include <stdio.h>
#include <stdlib.h>

int max_bitonic_sum(int* arr, int n) {
    if (arr == NULL || n == 0) {
        return 0;
    }
    if (n == 1) {
        return arr[0];
    }

    int* msis = (int*)malloc(n * sizeof(int));
    int* msds = (int*)malloc(n * sizeof(int));

    if (msis == NULL || msds == NULL) {
        free(msis);
        free(msds);
        return 0;
    }

    for (int i = 0; i < n; i++) {
        msis[i] = arr[i];
        msds[i] = arr[i];
    }

    for (int i = 1; i < n; i++) {
        for (int j = 0; j < i; j++) {
            if (arr[i] > arr[j] && msis[i] < msis[j] + arr[i]) {
                msis[i] = msis[j] + arr[i];
            }
        }
    }

    for (int i = n - 2; i >= 0; i--) {
        for (int j = n - 1; j > i; j--) {
            if (arr[i] > arr[j] && msds[i] < msds[j] + arr[i]) {
                msds[i] = msds[j] + arr[i];
            }
        }
    }

    int max_sum = msis[0] + msds[0] - arr[0];
    for (int i = 1; i < n; i++) {
        if (msis[i] + msds[i] - arr[i] > max_sum) {
            max_sum = msis[i] + msds[i] - arr[i];
        }
    }

    free(msis);
    free(msds);

    return max_sum;
}

int main() {
    int arr1[] = {1, 15, 51, 45, 33, 100, 12, 18, 9};
    int n1 = sizeof(arr1) / sizeof(arr1[0]);
    printf("%d\n", max_bitonic_sum(arr1, n1));

    int arr2[] = {5, 4, 3, 2, 1};
    int n2 = sizeof(arr2) / sizeof(arr2[0]);
    printf("%d\n", max_bitonic_sum(arr2, n2));

    int arr3[] = {1, 2, 3, 4, 5};
    int n3 = sizeof(arr3) / sizeof(arr3[0]);
    printf("%d\n", max_bitonic_sum(arr3, n3));

    int arr4[] = {10};
    int n4 = sizeof(arr4) / sizeof(arr4[0]);
    printf("%d\n", max_bitonic_sum(arr4, n4));

    return 0;
}