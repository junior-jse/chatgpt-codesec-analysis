#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

void findMaxProductPair(int arr[], int n, int *first, int *second, long long *maxProduct) {
    if (n < 2) {
        *first = 0;
        *second = 0;
        *maxProduct = LLONG_MIN;
        return;
    }

    int max1 = INT_MIN, max2 = INT_MIN;
    int min1 = INT_MAX, min2 = INT_MAX;

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

    long long product1 = (long long)max1 * max2;
    long long product2 = (long long)min1 * min2;

    if (product1 > product2) {
        *first = max1;
        *second = max2;
        *maxProduct = product1;
    } else {
        *first = min1;
        *second = min2;
        *maxProduct = product2;
    }
}

int main(void) {
    int arr[] = {-10, -3, 5, 6, -2, 0};
    int n = sizeof(arr) / sizeof(arr[0]);
    int first, second;
    long long maxProduct;

    findMaxProductPair(arr, n, &first, &second, &maxProduct);

    if (maxProduct != LLONG_MIN) {
        printf("Pair: (%d, %d)\n", first, second);
        printf("Maximum Product: %lld\n", maxProduct);
    } else {
        printf("Array has fewer than 2 elements\n");
    }

    return 0;
}