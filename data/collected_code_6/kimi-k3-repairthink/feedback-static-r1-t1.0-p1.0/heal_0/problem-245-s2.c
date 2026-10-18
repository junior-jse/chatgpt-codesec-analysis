#include <stdio.h>
#include <stdlib.h>

int maxSumBitonicSubseq(int arr[], int n) {
    if (arr == NULL || n <= 0) {
        return 0;
    }
    
    int *msis = (int *)malloc(n * sizeof(int));
    int *msds = (int *)malloc(n * sizeof(int));
    
    if (msis == NULL || msds == NULL) {
        free(msis);
        free(msds);
        return 0;
    }
    
    for (int i = 0; i < n; i++) {
        msis[i] = arr[i];
        for (int j = 0; j < i; j++) {
            if (arr[i] > arr[j] && msis[i] < msis[j] + arr[i]) {
                msis[i] = msis[j] + arr[i];
            }
        }
    }
    
    for (int i = n - 1; i >= 0; i--) {
        msds[i] = arr[i];
        for (int j = n - 1; j > i; j--) {
            if (arr[i] > arr[j] && msds[i] < msds[j] + arr[i]) {
                msds[i] = msds[j] + arr[i];
            }
        }
    }
    
    int max_sum = msis[0] + msds[0] - arr[0];
    for (int i = 1; i < n; i++) {
        int sum = msis[i] + msds[i] - arr[i];
        if (sum > max_sum) {
            max_sum = sum;
        }
    }
    
    free(msis);
    free(msds);
    
    return max_sum;
}

int main(void) {
    int arr[] = {1, 15, 51, 45, 33, 100, 12, 18, 9};
    int n = sizeof(arr) / sizeof(arr[0]);
    
    int result = maxSumBitonicSubseq(arr, n);
    printf("Maximum sum of bi-tonic sub-sequence: %d\n", result);
    
    return 0;
}