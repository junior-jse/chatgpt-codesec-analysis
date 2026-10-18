#include <stdio.h>
#include <stdlib.h>

/* Possible weaknesses found:
 *  Parameter 'arr' can be declared as const array [constParameter]
 */
int longest_bitonic(int arr[], int n) {
    if (n == 0) return 0;
    
    int *lis = (int *)malloc(n * sizeof(int));
    int *lds = (int *)malloc(n * sizeof(int));
    
    if (!lis || !lds) {
        free(lis);
        free(lds);
        return 0;
    }
    
    for (int i = 0; i < n; i++) {
        lis[i] = 1;
        lds[i] = 1;
    }
    
    for (int i = 1; i < n; i++) {
        for (int j = 0; j < i; j++) {
            if (arr[j] < arr[i] && lis[i] < lis[j] + 1) {
                lis[i] = lis[j] + 1;
            }
        }
    }
    
    for (int i = n - 2; i >= 0; i--) {
        for (int j = n - 1; j > i; j--) {
            if (arr[j] < arr[i] && lds[i] < lds[j] + 1) {
                lds[i] = lds[j] + 1;
            }
        }
    }
    
    int max_len = 0;
    for (int i = 0; i < n; i++) {
        int current = lis[i] + lds[i] - 1;
        if (current > max_len) {
            max_len = current;
        }
    }
    
    free(lis);
    free(lds);
    
    return max_len;
}

int main() {
    int arr[] = {1, 11, 2, 10, 4, 5, 2, 1};
    int n = sizeof(arr) / sizeof(arr[0]);
    
    int result = longest_bitonic(arr, n);
    printf("%d\n", result);
    
    return 0;
}