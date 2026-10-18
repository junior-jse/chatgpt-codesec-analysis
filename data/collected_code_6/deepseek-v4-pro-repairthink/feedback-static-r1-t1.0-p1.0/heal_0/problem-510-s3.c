#include <stdio.h>
#include <stdlib.h>

int countSubsequencesProductLessThanK(int arr[], int n, int k) {
    if (k <= 1) return 0;
    
    int count = 0;
    int totalSubsequences = 1;
    for (int i = 0; i < n; i++) {
        totalSubsequences *= 2;
    }
    
    for (int mask = 1; mask < totalSubsequences; mask++) {
        long long product = 1;
        int overflow = 0;
        
        for (int i = 0; i < n; i++) {
            if (mask & (1 << i)) {
                if (arr[i] == 0) {
                    product = 0;
                    break;
                }
                if (product > k / arr[i]) {
                    overflow = 1;
                    break;
                }
                product *= arr[i];
            }
        }
        
        if (!overflow && product < k) {
            count++;
        }
    }
    
    return count;
}

int main() {
    int arr1[] = {1, 2, 3};
    int n1 = sizeof(arr1) / sizeof(arr1[0]);
    int k1 = 4;
    printf("Result: %d\n", countSubsequencesProductLessThanK(arr1, n1, k1));
    
    int arr2[] = {4, 8, 7, 2};
    int n2 = sizeof(arr2) / sizeof(arr2[0]);
    int k2 = 50;
    printf("Result: %d\n", countSubsequencesProductLessThanK(arr2, n2, k2));
    
    int arr3[] = {0, 1, 2, 3};
    int n3 = sizeof(arr3) / sizeof(arr3[0]);
    int k3 = 10;
    printf("Result: %d\n", countSubsequencesProductLessThanK(arr3, n3, k3));
    
    int arr4[] = {5};
    int n4 = sizeof(arr4) / sizeof(arr4[0]);
    int k4 = 10;
    printf("Result: %d\n", countSubsequencesProductLessThanK(arr4, n4, k4));
    
    return 0;
}