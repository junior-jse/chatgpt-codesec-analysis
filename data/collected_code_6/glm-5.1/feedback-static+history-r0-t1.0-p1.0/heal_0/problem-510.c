#include <stdio.h>
#include <stdlib.h>

int count_subsequences(int arr[], int n, int k) {
    if (k <= 1) {
        return 0;
    }
    int result = 0;
    long long product = 1;
    int left = 0;
    for (int right = 0; right < n; right++) {
        product *= arr[right];
        while (product >= k && left <= right) {
            product /= arr[left];
            left++;
        }
        result += (right - left + 1);
    }
    return result;
}

int main() {
    int n, k;
    if (scanf("%d", &n) != 1 || n < 0) {
        return 1;
    }
    if (n > 0) {
        int *arr = (int *)malloc(n * sizeof(int));
        if (arr == NULL) {
            return 1;
        }
        for (int i = 0; i < n; i++) {
            if (scanf("%d", &arr[i]) != 1 || arr[i] < 0) {
                free(arr);
                return 1;
            }
        }
        if (scanf("%d", &k) != 1) {
            free(arr);
            return 1;
        }
        int ans = count_subsequences(arr, n, k);
        printf("%d\n", ans);
        free(arr);
    } else {
        if (scanf("%d", &k) != 1) {
            return 1;
        }
        printf("0\n");
    }
    return 0;
}