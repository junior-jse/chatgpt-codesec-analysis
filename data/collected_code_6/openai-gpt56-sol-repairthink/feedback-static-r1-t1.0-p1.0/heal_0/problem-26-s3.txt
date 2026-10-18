#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *elements;
    size_t count;
} Tuple;

static bool tuple_list_has_all_k_elements(const Tuple *tuples,
                                          size_t tuple_count,
                                          size_t k)
{
    if (tuple_count > 0 && tuples == NULL) {
        return false;
    }

    for (size_t i = 0; i < tuple_count; ++i) {
        if (tuples[i].count != k) {
            return false;
        }

        if (k > 0 && tuples[i].elements == NULL) {
            return false;
        }
    }

    return true;
}

int main(void)
{
    int first[] = {1, 2, 3};
    int second[] = {4, 5, 6};
    int third[] = {7, 8, 9};

    Tuple tuples[] = {
        {first, sizeof(first) / sizeof(first[0])},
        {second, sizeof(second) / sizeof(second[0])},
        {third, sizeof(third) / sizeof(third[0])}
    };

    const size_t tuple_count = sizeof(tuples) / sizeof(tuples[0]);
    const size_t k = 3;

    if (tuple_list_has_all_k_elements(tuples, tuple_count, k)) {
        if (printf("All tuples have exactly %zu elements.\n", k) < 0) {
            return EXIT_FAILURE;
        }
    } else {
        if (printf("Not all tuples have exactly %zu elements.\n", k) < 0) {
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}