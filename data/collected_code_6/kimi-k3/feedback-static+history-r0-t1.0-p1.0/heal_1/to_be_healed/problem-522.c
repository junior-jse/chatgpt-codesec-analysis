#include <stdio.h>
#include <stdlib.h>

/* Possible weaknesses found:
 *  Parameter 'arr' can be declared as const array [constParameter]
 */
int longestBitonicSubsequence(int arr[], int n) {
    if (n <= 0) return 0;
    
    int *lis = (int*)malloc(n * sizeof(int));
    int *lds = (int*)malloc(n * sizeof(int));
    
    if (lis == NULL || lds == NULL) {
        free(lis);
        free(lds);
        return -1;
    }
    
    for (int i = 0; i < n; i++) {
        lis[i] = 1;
        lds[i] = 1;
    }
    
    for (int i = 1; i < n; i++) {
        for (int j = 0; j < i; j++) {
            if (arr[i] > arr[j] && lis[i] < lis[j] + 1) {
                lis[i] = lis[j] + 1;
            }
        }
    }
    
    for (int i = n - 2; i >= 0; i--) {
        for (int j = n - 1; j > i; j--) {
            if (arr[i] > arr[j] && lds[i] < lds[j] + 1) {
                lds[i] = lds[j] + 1;
            }
        }
    }
    
    int max = lis[0] + lds[0] - 1;
    for (int i = 1; i < n; i++) {
        int current = lis[i] + lds[i] - 1;
        if (current > max) {
            max = current;
        }
    }
    
    free(lis);
    free(lds);
    
    return max;
}

int main() {
    int arr[] = {12, 11, 40, 5, 3, 1};
    int n = sizeof(arr) / sizeof(arr[0]);
    
    int result = longestBitonicSubsequence(arr, n);
    
    if (result >= 0) {
        printf("Length of longest bitonic subsequence: %d\n", result);
    } else {
        printf("Error: Memory allocation failed\n");
        return 1;
    }
    
    return 0;
}