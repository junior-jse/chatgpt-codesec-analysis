#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int first;
    int second;
    int third;
} Tuple;

static int extract_nth(
    const Tuple *tuples,
    size_t tuple_count,
    size_t tuple_index,
    size_t element_index,
    int *result
) {
    if (tuples == NULL || result == NULL ||
        tuple_index >= tuple_count || element_index >= 3U) {
        return 0;
    }

    switch (element_index) {
        case 0U:
            *result = tuples[tuple_index].first;
            break;
        case 1U:
            *result = tuples[tuple_index].second;
            break;
        case 2U:
            *result = tuples[tuple_index].third;
            break;
        default:
            return 0;
    }

    return 1;
}

int main(void) {
    const Tuple tuples[] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    const size_t tuple_count = sizeof(tuples) / sizeof(tuples[0]);
    const size_t tuple_index = 1U;
    const size_t element_index = 2U;
    int value = 0;

    if (!extract_nth(
            tuples,
            tuple_count,
            tuple_index,
            element_index,
            &value)) {
        fputs("Unable to extract the requested element.\n", stderr);
        return EXIT_FAILURE;
    }

    if (printf("%d\n", value) < 0) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}