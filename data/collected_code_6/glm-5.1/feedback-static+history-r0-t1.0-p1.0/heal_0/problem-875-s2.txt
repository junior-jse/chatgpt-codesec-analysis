#include <stdio.h>
#include <stdlib.h>

int find_min_difference(int *tuples, size_t size) {
    if (size < 2) {
        return 0;
    }

    int min_diff = abs(tuples[0] - tuples[1]);

    for (size_t i = 1; i < size - 1; ++i) {
        int current_diff = abs(tuples[i] - tuples[i + 1]);
        if (current_diff < min_diff) {
            min_diff = current_diff;
        }
    }

    return min_diff;
}

int main(void) {
    int data1[] = {1, 5, 3, 19, 18, 25};
    size_t size1 = sizeof(data1) / sizeof(data1[0]);
    printf("%d\n", find_min_difference(data1, size1));

    int data2[] = {10, 20, 30, 40, 50};
    size_t size2 = sizeof(data2) / sizeof(data2[0]);
    printf("%d\n", find_min_difference(data2, size2));

    int data3[] = {7};
    size_t size3 = sizeof(data3) / sizeof(data3[0]);
    printf("%d\n", find_min_difference(data3, size3));

    int data4[] = {5, 5, 5, 5};
    size_t size4 = sizeof(data4) / sizeof(data4[0]);
    printf("%d\n", find_min_difference(data4, size4));

    return 0;
}