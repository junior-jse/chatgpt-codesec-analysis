#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int first;
    int second;
} Tuple;

static int rearrange_tuples(const Tuple *tuples,
                            size_t tuple_count,
                            const size_t *order,
                            size_t order_count,
                            Tuple **result)
{
    Tuple *arranged;
    unsigned char *used;
    size_t i;

    if (result == NULL) {
        return -1;
    }

    *result = NULL;

    if (tuple_count != order_count) {
        return -1;
    }

    if (tuple_count == 0U) {
        return 0;
    }

    if (tuples == NULL || order == NULL ||
        tuple_count > SIZE_MAX / sizeof(*arranged)) {
        return -1;
    }

    arranged = malloc(tuple_count * sizeof(*arranged));
    if (arranged == NULL) {
        return -1;
    }

    used = calloc(tuple_count, sizeof(*used));
    if (used == NULL) {
        free(arranged);
        return -1;
    }

    for (i = 0U; i < order_count; ++i) {
        if (order[i] >= tuple_count || used[order[i]] != 0U) {
            free(used);
            free(arranged);
            return -1;
        }

        arranged[i] = tuples[order[i]];
        used[order[i]] = 1U;
    }

    free(used);
    *result = arranged;
    return 0;
}

int main(void)
{
    const Tuple tuples[] = {
        {10, 20},
        {30, 40},
        {50, 60},
        {70, 80}
    };
    const size_t order[] = {2U, 0U, 3U, 1U};
    const size_t count = sizeof(tuples) / sizeof(tuples[0]);
    Tuple *arranged = NULL;
    size_t i;

    if (rearrange_tuples(tuples, count, order,
                         sizeof(order) / sizeof(order[0]),
                         &arranged) != 0) {
        fputs("Failed to rearrange tuples.\n", stderr);
        return EXIT_FAILURE;
    }

    for (i = 0U; i < count; ++i) {
        if (printf("(%d, %d)\n", arranged[i].first,
                   arranged[i].second) < 0) {
            free(arranged);
            return EXIT_FAILURE;
        }
    }

    free(arranged);
    return EXIT_SUCCESS;
}