#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int first;
    int second;
} Tuple;

static int count_occurrences(const Tuple *tuples, size_t tuple_count, int value)
{
    int count = 0;

    for (size_t i = 0; i < tuple_count; ++i) {
        if (tuples[i].first == value) {
            ++count;
        }
        if (tuples[i].second == value) {
            ++count;
        }
    }

    return count;
}

static int contains(const int *values, size_t count, int value)
{
    for (size_t i = 0; i < count; ++i) {
        if (values[i] == value) {
            return 1;
        }
    }

    return 0;
}

static int extract_single_elements(const Tuple *tuples,
                                   size_t tuple_count,
                                   int **result,
                                   size_t *result_count)
{
    int *single_elements = NULL;
    size_t capacity;

    if (result == NULL || result_count == NULL) {
        return 0;
    }

    *result = NULL;
    *result_count = 0;

    if (tuple_count == 0) {
        return 1;
    }

    if (tuples == NULL || tuple_count > SIZE_MAX / 2 ||
        tuple_count * 2 > SIZE_MAX / sizeof(*single_elements)) {
        return 0;
    }

    capacity = tuple_count * 2;
    single_elements = malloc(capacity * sizeof(*single_elements));
    if (single_elements == NULL) {
        return 0;
    }

    for (size_t i = 0; i < tuple_count; ++i) {
        const int candidates[2] = {tuples[i].first, tuples[i].second};

        for (size_t j = 0; j < 2; ++j) {
            if (count_occurrences(tuples, tuple_count, candidates[j]) == 1 &&
                !contains(single_elements, *result_count, candidates[j])) {
                single_elements[*result_count] = candidates[j];
                ++(*result_count);
            }
        }
    }

    if (*result_count == 0) {
        free(single_elements);
        return 1;
    }

    *result = single_elements;
    return 1;
}

int main(void)
{
    const Tuple tuples[] = {
        {1, 2},
        {2, 3},
        {4, 5},
        {5, 6}
    };
    const size_t tuple_count = sizeof(tuples) / sizeof(tuples[0]);
    int *single_elements = NULL;
    size_t single_count = 0;

    if (!extract_single_elements(tuples, tuple_count,
                                 &single_elements, &single_count)) {
        fputs("Failed to extract single elements.\n", stderr);
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < single_count; ++i) {
        printf("%d%s", single_elements[i],
               i + 1 == single_count ? "\n" : " ");
    }

    free(single_elements);
    return EXIT_SUCCESS;
}