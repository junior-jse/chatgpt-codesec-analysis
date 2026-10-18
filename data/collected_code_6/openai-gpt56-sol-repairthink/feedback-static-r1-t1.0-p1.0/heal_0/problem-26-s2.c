#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

typedef struct {
    const void *elements;
    size_t count;
} Tuple;

static bool tuple_list_has_k_elements(const Tuple *tuples,
                                      size_t tuple_count,
                                      size_t k)
{
    size_t i;

    if (tuples == NULL && tuple_count != 0U) {
        return false;
    }

    for (i = 0U; i < tuple_count; ++i) {
        if (tuples[i].count != k) {
            return false;
        }

        if (k != 0U && tuples[i].elements == NULL) {
            return false;
        }
    }

    return true;
}

int main(void)
{
    const int first[] = {1, 2, 3};
    const int second[] = {4, 5, 6};
    const Tuple tuples[] = {
        {first, sizeof(first) / sizeof(first[0])},
        {second, sizeof(second) / sizeof(second[0])}
    };
    const size_t tuple_count = sizeof(tuples) / sizeof(tuples[0]);
    const size_t k = 3U;

    if (tuple_list_has_k_elements(tuples, tuple_count, k)) {
        if (printf("All tuples have %zu elements.\n", k) < 0) {
            return 1;
        }
    } else {
        if (printf("Not all tuples have %zu elements.\n", k) < 0) {
            return 1;
        }
    }

    return 0;
}