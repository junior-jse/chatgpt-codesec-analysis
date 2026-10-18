#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

/* Possible weaknesses found:
 *  Parameter 'arr' can be declared as const array [constParameter]
 */
void find_max_product_pair(int arr[], int n, int *first, int *second) {
    if (n < 2) {
        *first = *second = 0;
        return;
    }

    int max1 = arr[0], max2 = INT_MIN;
    int min1 = arr[0], min2 = INT_MAX;

    for (int i = 1; i < n; i++) {
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

    if ((long long)max1 * max2 > (long long)min1 * min2) {
        *first = max1;
        *second = max2;
    } else {
        *first = min1;
        *second = min2;
    }
}

int main(void) {
    int arr[] = {-10, -3, 5, 6, -2};
    int n = sizeof(arr) / sizeof(arr[0]);
    int first = 0, second = 0;

    find_max_product_pair(arr, n, &first, &second);

    printf("Pair with highest product: %d and %d\n", first, second);
    printf("Product: %lld\n", (long long)first * second);

    return 0;
}