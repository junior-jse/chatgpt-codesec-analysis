#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

void find_max_product_pair(int *arr, int n, int *res) {
    if (arr == NULL || n < 2) {
        res[0] = INT_MIN;
        res[1] = INT_MIN;
        return;
    }

    int max1 = INT_MIN;
    int max2 = INT_MIN;
    int min1 = INT_MAX;
    int min2 = INT_MAX;

    for (int i = 0; i < n; i++) {
        if (arr[i] > max1) {
            max2 = max1;
            max1 = arr[i];
        } else if (arr[i] > max2) {
            max2 = arr[i];
        }

        if (arr[i] < min1) {
            min2 = min1;
            min1 = arr[i];
        } else if (arr[i] < min2) {
            min2 = arr[i];
        }
    }

    if (max1 * max2 > min1 * min2) {
        res[0] = max1;
        res[1] = max2;
    } else {
        res[0] = min1;
        res[1] = min2;
    }
}

int main(void) {
    int arr1[] = {1, 4, 3, 6, 2, 7, 5};
    int n1 = sizeof(arr1) / sizeof(arr1[0]);
    int res1[2];
    find_max_product_pair(arr1, n1, res1);
    printf("Pair with highest product: (%d, %d)\n", res1[0], res1[1]);

    int arr2[] = {-1, -3, -4, 2, 0, -5};
    int n2 = sizeof(arr2) / sizeof(arr2[0]);
    int res2[2];
    find_max_product_pair(arr2, n2, res2);
    printf("Pair with highest product: (%d, %d)\n", res2[0], res2[1]);

    return 0;
}