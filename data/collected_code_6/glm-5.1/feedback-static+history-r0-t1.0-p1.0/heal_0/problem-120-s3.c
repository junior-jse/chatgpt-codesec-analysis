#include <stdio.h>
#include <stdlib.h>

int find_max_product(int tuples[][2], size_t count) {
    if (count == 0) {
        exit(EXIT_FAILURE);
    }
    int max_product = tuples[0][0] * tuples[0][1];
    for (size_t i = 1; i < count; i++) {
        int product = tuples[i][0] * tuples[i][1];
        if (product > max_product) {
            max_product = product;
        }
    }
    return max_product;
}

int main(void) {
    int data[][2] = {{1, 5}, {3, 4}, {-6, 2}, {7, -8}};
    size_t count = sizeof(data) / sizeof(data[0]);
    int result = find_max_product(data, count);
    printf("%d\n", result);
    return 0;
}