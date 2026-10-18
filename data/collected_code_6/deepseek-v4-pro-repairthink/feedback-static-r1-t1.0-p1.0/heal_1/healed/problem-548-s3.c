#include <stdio.h>
#include <stdlib.h>

int longest_increasing_subsequence(const int *seq, int n) {
    if (n <= 0) return 0;
    
    int *lis = (int *)malloc(n * sizeof(int));
    if (lis == NULL) return -1;
    
    for (int i = 0; i < n; i++) {
        lis[i] = 1;
    }
    
    for (int i = 1; i < n; i++) {
        for (int j = 0; j < i; j++) {
            if (seq[i] > seq[j] && lis[i] < lis[j] + 1) {
                lis[i] = lis[j] + 1;
            }
        }
    }
    
    int max = 0;
    for (int i = 0; i < n; i++) {
        if (lis[i] > max) {
            max = lis[i];
        }
    }
    
    free(lis);
    return max;
}

int main(void) {
    int arr[] = {10, 22, 9, 33, 21, 50, 41, 60, 80};
    int n = sizeof(arr) / sizeof(arr[0]);
    int result = longest_increasing_subsequence(arr, n);
    printf("%d\n", result);
    return 0;
}