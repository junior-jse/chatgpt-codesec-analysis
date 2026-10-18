#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct {
    int a;
    int b;
} Tuple;

long long max_product_from_tuples(Tuple *tuples, size_t count) {
    if (tuples == NULL || count == 0) {
        return LLONG_MIN;
    }

    long long max_prod = LLONG_MIN;

    for (size_t i = 0; i < count; i++) {
        long long product = (long long)tuples[i].a * (long long)tuples[i].b;
        if (product > max_prod) {
            max_prod = product;
        }
    }

    return max_prod;
}

int main(void) {
    Tuple tuples[] = {
        {2, 5},
        {-10, -4},
        {3, 3},
        {-5, 2}
    };

    size_t count = sizeof(tuples) / sizeof(tuples[0]);

    long long result = max_product_from_tuples(tuples, count);

    printf("%lld\n", result);

    return 0;
}