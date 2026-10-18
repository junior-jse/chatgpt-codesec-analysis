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

static Tuple *remove_matching_tuples(
    const Tuple *first,
    size_t first_count,
    const Tuple *second,
    size_t second_count,
    size_t *result_count)
{
    Tuple *result;
    size_t kept = 0;

    if (result_count == NULL ||
        (first == NULL && first_count != 0) ||
        (second == NULL && second_count != 0)) {
        return NULL;
    }

    *result_count = 0;

    if (first_count == 0) {
        return NULL;
    }

    if (first_count > SIZE_MAX / sizeof(*result)) {
        return NULL;
    }

    result = malloc(first_count * sizeof(*result));
    if (result == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < first_count; ++i) {
        bool matched = false;

        for (size_t j = 0; j < second_count; ++j) {
            if (tuples_equal(&first[i], &second[j])) {
                matched = true;
                break;
            }
        }

        if (!matched) {
            result[kept++] = first[i];
        }
    }

    if (kept == 0) {
        free(result);
        return NULL;
    }

    *result_count = kept;
    return result;
}

int main(void)
{
    const Tuple first[] = {
        {1, 2},
        {3, 4},
        {5, 6},
        {7, 8}
    };
    const Tuple second[] = {
        {3, 4},
        {7, 8}
    };
    const size_t first_count = sizeof(first) / sizeof(first[0]);
    const size_t second_count = sizeof(second) / sizeof(second[0]);
    size_t result_count = 0;
    Tuple *result = remove_matching_tuples(
        first,
        first_count,
        second,
        second_count,
        &result_count
    );

    if (result == NULL && result_count == 0) {
        bool all_removed = true;

        for (size_t i = 0; i < first_count; ++i) {
            bool matched = false;

            for (size_t j = 0; j < second_count; ++j) {
                if (tuples_equal(&first[i], &second[j])) {
                    matched = true;
                    break;
                }
            }

            if (!matched) {
                all_removed = false;
                break;
            }
        }

        if (!all_removed) {
            fputs("Failed to allocate result storage.\n", stderr);
            return EXIT_FAILURE;
        }
    }

    for (size_t i = 0; i < result_count; ++i) {
        if (printf("(%d, %d)\n", result[i].first, result[i].second) < 0) {
            free(result);
            return EXIT_FAILURE;
        }
    }

    free(result);
    return EXIT_SUCCESS;
}