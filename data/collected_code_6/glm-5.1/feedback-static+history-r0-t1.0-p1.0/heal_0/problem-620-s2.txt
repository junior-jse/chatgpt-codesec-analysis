#include <stdio.h>
#include <stdlib.h>

int compare(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

int largest_divisible_subset(int *arr, int n) {
    if (n == 0) return 0;
    qsort(arr, n, sizeof(int), compare);
    int *dp = (int *)malloc(n * sizeof(int));
    if (dp == NULL) return 0;
    for (int i = 0; i < n; i++) dp[i] = 1;
    int max_size = 1;
    for (int i = 1; i < n; i++) {
        for (int j = 0; j < i; j++) {
            if (arr[i] % arr[j] == 0 && dp[j] + 1 > dp[i]) {
                dp[i] = dp[j] + 1;
                if (dp[i] > max_size) max_size = dp[i];
            }
        }
    }
    free(dp);
    return max_size;
}

int main(void) {
    int arr1[] = {1, 2, 3};
    int n1 = sizeof(arr1) / sizeof(arr1[0]);
    printf("%d\n", largest_divisible_subset(arr1, n1));
    int arr2[] = {1, 2, 4, 8};
    int n2 = sizeof(arr2) / sizeof(arr2[0]);
    printf("%d\n", largest_divisible_subset(arr2, n2));
    int arr3[] = {5, 7, 11};
    int n3 = sizeof(arr3) / sizeof(arr3[0]);
    printf("%d\n", largest_divisible_subset(arr3, n3));
    return 0;
}