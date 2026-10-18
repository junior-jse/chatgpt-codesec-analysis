#include <stdio.h>
#include <stdlib.h>

int max_sum_bitonic(int arr[], int n) {
    if (n <= 0) return 0;
    if (n == 1) return arr[0];

    int *inc = (int *)malloc(n * sizeof(int));
    int *dec = (int *)malloc(n * sizeof(int));
    if (!inc || !dec) {
        free(inc);
        free(dec);
        return 0;
    }

    for (int i = 0; i < n; i++) {
        inc[i] = arr[i];
        dec[i] = arr[i];
    }

    for (int i = 1; i < n; i++) {
        for (int j = 0; j < i; j++) {
            if (arr[i] > arr[j] && inc[i] < inc[j] + arr[i]) {
                inc[i] = inc[j] + arr[i];
            }
        }
    }

    for (int i = n - 2; i >= 0; i--) {
        for (int j = n - 1; j > i; j--) {
            if (arr[i] > arr[j] && dec[i] < dec[j] + arr[i]) {
                dec[i] = dec[j] + arr[i];
            }
        }
    }

    int max_sum = 0;
    for (int i = 0; i < n; i++) {
        int total = inc[i] + dec[i] - arr[i];
        if (total > max_sum) {
            max_sum = total;
        }
    }

    free(inc);
    free(dec);
    return max_sum;
}

int main(void) {
    int arr1[] = {1, 15, 51, 45, 33, 100, 12, 18, 9};
    int n1 = sizeof(arr1) / sizeof(arr1[0]);
    printf("%d\n", max_sum_bitonic(arr1, n1));

    int arr2[] = {80, 60, 30, 40, 20, 10};
    int n2 = sizeof(arr2) / sizeof(arr2[0]);
    printf("%d\n", max_sum_bitonic(arr2, n2));

    int arr3[] = {10, 20, 30, 40};
    int n3 = sizeof(arr3) / sizeof(arr3[0]);
    printf("%d\n", max_sum_bitonic(arr3, n3));

    int arr4[] = {5};
    int n4 = sizeof(arr4) / sizeof(arr4[0]);
    printf("%d\n", max_sum_bitonic(arr4, n4));

    int arr5[] = {50, 40, 30, 20, 10};
    int n5 = sizeof(arr5) / sizeof(arr5[0]);
    printf("%d\n", max_sum_bitonic(arr5, n5));

    return 0;
}