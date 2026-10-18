#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct {
    int first;
    int second;
} Tuple;

int maxProduct(Tuple *tuples, int size) {
    if (tuples == NULL || size <= 0) {
        return INT_MIN;
    }

    int max_product = INT_MIN;

    for (int i = 0; i < size; i++) {
        for (int j = i + 1; j < size; j++) {
            int product1 = tuples[i].first * tuples[j].first;
            int product2 = tuples[i].first * tuples[j].second;
            int product3 = tuples[i].second * tuples[j].first;
            int product4 = tuples[i].second * tuples[j].second;

            if (product1 > max_product) max_product = product1;
            if (product2 > max_product) max_product = product2;
            if (product3 > max_product) max_product = product3;
            if (product4 > max_product) max_product = product4;
        }
    }

    return max_product;
}

int main(void) {
    Tuple tuples[] = {
        {1, 2},
        {3, 4},
        {-5, 6},
        {7, -8}
    };

    int size = sizeof(tuples) / sizeof(tuples[0]);
    int result = maxProduct(tuples, size);

    if (result != INT_MIN) {
        printf("Maximum product: %d\n", result);
    } else {
        printf("Invalid input\n");
    }

    return 0;
}