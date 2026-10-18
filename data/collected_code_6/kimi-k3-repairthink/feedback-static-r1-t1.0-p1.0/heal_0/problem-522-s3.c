#include <stdio.h>
#include <stdlib.h>

int longestBitonicSubsequence(const int *arr, int n) {
    if (arr == NULL || n <= 0) {
        return 0;
    }

    int *lis = (int *)malloc((size_t)n * sizeof(int));
    int *lds = (int *)malloc((size_t)n * sizeof(int));

    if (lis == NULL || lds == NULL) {
        free(lis);
        free(lds);
        return 0;
    }

    int i, j;

    for (i = 0; i < n; i++) {
        lis[i] = 1;
        lds[i] = 1;
    }

    for (i = 1; i < n; i++) {
        for (j = 0; j < i; j++) {
            if (arr[i] > arr[j] && lis[i] < lis[j] + 1) {
                lis[i] = lis[j] + 1;
            }
        }
    }

    for (i = n - 2; i >= 0; i--) {
        for (j = n - 1; j > i; j--) {
            if (arr[i] > arr[j] && lds[i] < lds[j] + 1) {
                lds[i] = lds[j] + 1;
            }
        }
    }

    int max = 0;
    for (i = 0; i < n; i++) {
        if (max < lis[i] + lds[i] - 1) {
            max = lis[i] + lds[i] - 1;
        }
    }

    free(lis);
    free(lds);

    return max;
}

int main(void) {
    int arr[] = {12, 11, 40, 5, 3, 1};
    int n = (int)(sizeof(arr) / sizeof(arr[0]));

    int result = longestBitonicSubsequence(arr, n);
    printf("Length of Longest Bitonic Subsequence: %d\n", result);

    return 0;
}