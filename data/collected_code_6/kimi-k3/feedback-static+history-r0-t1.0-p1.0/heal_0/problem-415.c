#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

bool find_max_product_pair(const int *arr, size_t size, int *first, int *second)
{
    if (arr == NULL || first == NULL || second == NULL || size < 2) {
        return false;
    }

    int max1 = INT_MIN;
    int max2 = INT_MIN;
    int min1 = INT_MAX;
    int min2 = INT_MAX;

    for (size_t i = 0; i < size; i++) {
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

    long long product_pos = (long long)max1 * max2;
    long long product_neg = (long long)min1 * min2;

    if (product_pos >= product_neg) {
        *first = max1;
        *second = max2;
    } else {
        *first = min1;
        *second = min2;
    }

    return true;
}

int main(void)
{
    int arr[] = {1, 4, 3, -6, 7, -8, 2};
    size_t size = sizeof(arr) / sizeof(arr[0]);
    int first = 0;
    int second = 0;

    if (find_max_product_pair(arr, size, &first, &second)) {
        printf("Pair: (%d, %d)\n", first, second);
        printf("Product: %lld\n", (long long)first * second);
    } else {
        fprintf(stderr, "Error: Invalid input\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}