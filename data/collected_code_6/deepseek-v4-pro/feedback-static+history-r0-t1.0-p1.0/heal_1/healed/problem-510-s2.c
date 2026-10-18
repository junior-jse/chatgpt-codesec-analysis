#include <stdio.h>
#include <stdlib.h>

int countSubsequencesProductLessThanK(const int arr[], int n, int k) {
    if (k <= 1) return 0;
    
    int count = 0;
    int prod = 1;
    int left = 0;
    
    for (int right = 0; right < n; right++) {
        if (arr[right] >= k) {
            prod = 1;
            left = right + 1;
            continue;
        }
        
        prod *= arr[right];
        
        while (prod >= k && left <= right) {
            prod /= arr[left];
            left++;
        }
        
        count += (right - left + 1);
    }
    
    return count;
}

int main(void) {
    int arr[] = {1, 2, 3, 4};
    int n = sizeof(arr) / sizeof(arr[0]);
    int k = 10;
    
    int result = countSubsequencesProductLessThanK(arr, n, k);
    printf("%d\n", result);
    
    int arr2[] = {5, 1, 2, 3};
    int n2 = sizeof(arr2) / sizeof(arr2[0]);
    int k2 = 10;
    
    int result2 = countSubsequencesProductLessThanK(arr2, n2, k2);
    printf("%d\n", result2);
    
    int arr3[] = {0, 0, 0};
    int n3 = sizeof(arr3) / sizeof(arr3[0]);
    int k3 = 1;
    
    int result3 = countSubsequencesProductLessThanK(arr3, n3, k3);
    printf("%d\n", result3);
    
    return 0;
}