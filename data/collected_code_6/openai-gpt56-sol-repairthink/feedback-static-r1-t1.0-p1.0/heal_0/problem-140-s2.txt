#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    int first;
    int second;
} Tuple;

static bool tuples_equal(const Tuple *left, const Tuple *right)
{
    return left->first == right->first && left->second == right->second;
}

static int extract_singletons(const Tuple *input, size_t count,
                              Tuple **output, size_t *output_count)
{
    Tuple *result = NULL;
    size_t unique_count = 0;

    if (output == NULL || output_count == NULL ||
        (input == NULL && count != 0)) {
        return -1;
    }

    *output = NULL;
    *output_count = 0;

    for (size_t i = 0; i < count; ++i) {
        size_t occurrences = 0;

        for (size_t j = 0; j < count; ++j) {
            if (tuples_equal(&input[i], &input[j])) {
                ++occurrences;
            }
        }

        if (occurrences == 1) {
            ++unique_count;
        }
    }

    if (unique_count == 0) {
        return 0;
    }

    if (unique_count > SIZE_MAX / sizeof(*result)) {
        return -1;
    }

    result = malloc(unique_count * sizeof(*result));
    if (result == NULL) {
        return -1;
    }

    size_t index = 0;

    for (size_t i = 0; i < count; ++i) {
        size_t occurrences = 0;

        for (size_t j = 0; j < count; ++j) {
            if (tuples_equal(&input[i], &input[j])) {
                ++occurrences;
            }
        }

        if (occurrences == 1) {
            result[index++] = input[i];
        }
    }

    *output = result;
    *output_count = unique_count;
    return 0;
}

int main(void)
{
    const Tuple tuples[] = {
        {1, 2},
        {3, 4},
        {1, 2},
        {5, 6},
        {7, 8},
        {3, 4}
    };
    const size_t tuple_count = sizeof(tuples) / sizeof(tuples[0]);
    Tuple *singletons = NULL;
    size_t singleton_count = 0;

    if (extract_singletons(tuples, tuple_count,
                           &singletons, &singleton_count) != 0) {
        fputs("Failed to extract singleton tuples.\n", stderr);
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < singleton_count; ++i) {
        if (printf("(%d, %d)\n",
                   singletons[i].first, singletons[i].second) < 0) {
            free(singletons);
            return EXIT_FAILURE;
        }
    }

    free(singletons);
    return EXIT_SUCCESS;
}