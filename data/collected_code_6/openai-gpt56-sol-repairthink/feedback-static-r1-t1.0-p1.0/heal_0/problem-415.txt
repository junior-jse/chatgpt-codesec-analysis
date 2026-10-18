#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

static int find_highest_product_pair(const int *array, size_t length,
                                     int *first, int *second)
{
    int largest;
    int second_largest;
    int smallest;
    int second_smallest;
    size_t i;

    if (array == NULL || first == NULL || second == NULL || length < 2U) {
        return 0;
    }

    if (array[0] >= array[1]) {
        largest = array[0];
        second_largest = array[1];
    } else {
        largest = array[1];
        second_largest = array[0];
    }

    if (array[0] <= array[1]) {
        smallest = array[0];
        second_smallest = array[1];
    } else {
        smallest = array[1];
        second_smallest = array[0];
    }

    for (i = 2U; i < length; ++i) {
        if (array[i] > largest) {
            second_largest = largest;
            largest = array[i];
        } else if (array[i] > second_largest) {
            second_largest = array[i];
        }

        if (array[i] < smallest) {
            second_smallest = smallest;
            smallest = array[i];
        } else if (array[i] < second_smallest) {
            second_smallest = array[i];
        }
    }

    if ((long long)largest * second_largest >=
        (long long)smallest * second_smallest) {
        *first = largest;
        *second = second_largest;
    } else {
        *first = smallest;
        *second = second_smallest;
    }

    return 1;
}

int main(void)
{
    int values[] = {-10, -3, 5, 6, -2};
    const size_t length = sizeof(values) / sizeof(values[0]);
    int first;
    int second;

    if (!find_highest_product_pair(values, length, &first, &second)) {
        fputs("Unable to find a pair.\n", stderr);
        return EXIT_FAILURE;
    }

    if (printf("Pair: %d, %d\nProduct: %lld\n",
               first, second, (long long)first * second) < 0) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}