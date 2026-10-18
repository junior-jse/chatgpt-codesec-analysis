#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

/* Possible weaknesses found:
 *  Parameter 'arr' can be declared as pointer to const [constParameterPointer]
 */
void find_max_product_pair(int *arr, int size) {
    if (arr == NULL || size < 2) {
        printf("Invalid input or array size too small.\n");
        return;
    }

    int max1 = INT_MIN;
    int max2 = INT_MIN;
    int min1 = INT_MAX;
    int min2 = INT_MAX;

    for (int i = 0; i < size; i++) {
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
        printf("Pair with highest product is (%d, %d)\n", max1, max2);
    } else {
        printf("Pair with highest product is (%d, %d)\n", min1, min2);
    }
}

int main(void) {
    int arr[] = {1, 4, 3, 6, 2, 7, -5, -8};
    int size = sizeof(arr) / sizeof(arr[0]);

    find_max_product_pair(arr, size);

    return 0;
}