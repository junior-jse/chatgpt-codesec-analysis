#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

int find_highest_product_pair(const int *array, size_t length, int *first, int *second)
{
    int max1;
    int max2;
    int min1;
    int min2;

    if (array == NULL || first == NULL || second == NULL || length < 2U) {
        return 0;
    }

    if (array[0] >= array[1]) {
        max1 = array[0];
        max2 = array[1];
        min1 = array[1];
        min2 = array[0];
    } else {
        max1 = array[1];
        max2 = array[0];
        min1 = array[0];
        min2 = array[1];
    }

    for (size_t i = 2U; i < length; ++i) {
        if (array[i] > max1) {
            max2 = max1;
            max1 = array[i];
        } else if (array[i] > max2) {
            max2 = array[i];
        }

        if (array[i] < min1) {
            min2 = min1;
            min1 = array[i];
        } else if (array[i] < min2) {
            min2 = array[i];
        }
    }

    if ((long long)max1 * max2 >= (long long)min1 * min2) {
        *first = max1;
        *second = max2;
    } else {
        *first = min1;
        *second = min2;
    }

    return 1;
}

int main(void)
{
    int array[] = {1, -10, -20, 4, 8, 12};
    const size_t length = sizeof(array) / sizeof(array[0]);
    int first;
    int second;

    if (!find_highest_product_pair(array, length, &first, &second)) {
        fputs("Unable to find a pair.\n", stderr);
        return EXIT_FAILURE;
    }

    if (printf("Pair with highest product: %d and %d\n", first, second) < 0) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}