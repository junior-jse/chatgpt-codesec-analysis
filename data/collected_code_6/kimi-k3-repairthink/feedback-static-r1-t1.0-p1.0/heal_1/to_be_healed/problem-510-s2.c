#include <stdio.h>
#include <stdlib.h>

/* Possible weaknesses found:
 *  Parameter 'arr' can be declared as const array [constParameter]
 */
int countSubsequences(int arr[], int n, int k) {
    int left = 0, right = n - 1;
    int count = 0;
    
    while (left <= right) {
        if (arr[left] * arr[right] < k) {
            count += (1 << (right - left));
            left++;
        } else {
            right--;
        }
    }
    
    return count;
}

int main() {
    int arr[] = {1, 2, 3, 4};
    int n = sizeof(arr) / sizeof(arr[0]);
    int k = 10;
    
    int result = countSubsequences(arr, n, k);
    printf("Number of subsequences with product smaller than %d: %d\n", k, result);
    
    return 0;
}