#include <stdio.h>
#include <stdlib.h>

/* Possible weaknesses found:
 *  Parameter 'arr' can be declared as const array [constParameter]
 */
int count_subsequences(int arr[], int n, int k) {
    if (k <= 1) {
        return 0;
    }
    int res = 0;
    long long prod = 1;
    int left = 0;
    for (int right = 0; right < n; right++) {
        prod *= arr[right];
        while (prod >= k && left <= right) {
            prod /= arr[left];
            left++;
        }
        if (left <= right) {
            res += (right - left + 1);
        }
    }
    return res;
}

int main() {
    int n, k;
    if (scanf("%d", &n) != 1) {
        return 1;
    }
    if (n < 0) {
        return 1;
    }
    int *arr = (int *)malloc(n * sizeof(int));
    if (arr == NULL) {
        return 1;
    }
    for (int i = 0; i < n; i++) {
        if (scanf("%d", &arr[i]) != 1) {
            free(arr);
            return 1;
        }
    }
    if (scanf("%d", &k) != 1) {
        free(arr);
        return 1;
    }
    int result = count_subsequences(arr, n, k);
    printf("%d\n", result);
    free(arr);
    return 0;
}