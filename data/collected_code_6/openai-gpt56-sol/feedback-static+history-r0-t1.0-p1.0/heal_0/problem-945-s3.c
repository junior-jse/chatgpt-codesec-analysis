#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int first;
    int second;
} Tuple;

static int compare_tuples(const void *lhs, const void *rhs)
{
    const Tuple *a = lhs;
    const Tuple *b = rhs;

    if (a->first < b->first) {
        return -1;
    }
    if (a->first > b->first) {
        return 1;
    }
    if (a->second < b->second) {
        return -1;
    }
    if (a->second > b->second) {
        return 1;
    }
    return 0;
}

static int tuples_to_set(const Tuple *tuples, size_t count,
                         Tuple **set, size_t *set_count)
{
    Tuple *result;
    size_t read_index;
    size_t write_index;

    if (set == NULL || set_count == NULL || (tuples == NULL && count != 0)) {
        return -1;
    }

    *set = NULL;
    *set_count = 0;

    if (count == 0) {
        return 0;
    }

    if (count > SIZE_MAX / sizeof(*result)) {
        return -1;
    }

    result = malloc(count * sizeof(*result));
    if (result == NULL) {
        return -1;
    }

    for (read_index = 0; read_index < count; ++read_index) {
        result[read_index] = tuples[read_index];
    }

    qsort(result, count, sizeof(*result), compare_tuples);

    write_index = 1;
    for (read_index = 1; read_index < count; ++read_index) {
        if (compare_tuples(&result[read_index], &result[write_index - 1]) != 0) {
            result[write_index++] = result[read_index];
        }
    }

    *set = result;
    *set_count = write_index;
    return 0;
}

int main(void)
{
    const Tuple tuples[] = {
        {1, 2},
        {3, 4},
        {1, 2},
        {5, 6},
        {3, 4},
        {2, 8}
    };
    Tuple *set = NULL;
    size_t set_count = 0;
    size_t index;

    if (tuples_to_set(tuples, sizeof(tuples) / sizeof(tuples[0]),
                      &set, &set_count) != 0) {
        fputs("Failed to convert tuples to a set.\n", stderr);
        return EXIT_FAILURE;
    }

    for (index = 0; index < set_count; ++index) {
        if (printf("(%d, %d)\n", set[index].first, set[index].second) < 0) {
            free(set);
            return EXIT_FAILURE;
        }
    }

    free(set);
    return EXIT_SUCCESS;
}