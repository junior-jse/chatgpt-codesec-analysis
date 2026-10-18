#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int x;
    int y;
} Tuple;

int max_pair_product(Tuple *tuples, int n) {
    if (tuples == NULL || n <= 0) {
        return 0;
    }

    int max_x = tuples[0].x;
    int max_y = tuples[0].y;
    int product = max_x * max_y;

    for (int i = 1; i < n; i++) {
        int current_product = tuples[i].x * tuples[i].y;
        if (current_product > product) {
            product = current_product;
            max_x = tuples[i].x;
            max_y = tuples[i].y;
        }
    }

    return product;
}

int main(void) {
    Tuple tuples[] = {
        {3, 4},
        {1, 2},
        {5, -1},
        {0, 7},
        {-2, -6},
        {4, 4}
    };
    int n = sizeof(tuples) / sizeof(tuples[0]);

    int result = max_pair_product(tuples, n);
    printf("Maximum product from pairs: %d\n", result);

    return 0;
}