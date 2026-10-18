#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int first;
    int second;
} Tuple;

static int tuples_equal(const Tuple *a, const Tuple *b)
{
    return a->first == b->first && a->second == b->second;
}

static int remove_matching_tuples(
    const Tuple *first,
    size_t first_count,
    const Tuple *second,
    size_t second_count,
    Tuple **result,
    size_t *result_count)
{
    Tuple *output;
    size_t kept = 0;

    if (result == NULL || result_count == NULL ||
        (first_count > 0 && first == NULL) ||
        (second_count > 0 && second == NULL)) {
        return -1;
    }

    *result = NULL;
    *result_count = 0;

    if (first_count == 0) {
        return 0;
    }

    if (first_count > SIZE_MAX / sizeof(*output)) {
        return -1;
    }

    output = malloc(first_count * sizeof(*output));
    if (output == NULL) {
        return -1;
    }

    for (size_t i = 0; i < first_count; ++i) {
        int matched = 0;

        for (size_t j = 0; j < second_count; ++j) {
            if (tuples_equal(&first[i], &second[j])) {
                matched = 1;
                break;
            }
        }

        if (!matched) {
            output[kept++] = first[i];
        }
    }

    if (kept == 0) {
        free(output);
        output = NULL;
    } else {
        Tuple *smaller = realloc(output, kept * sizeof(*output));
        if (smaller != NULL) {
            output = smaller;
        }
    }

    *result = output;
    *result_count = kept;
    return 0;
}

int main(void)
{
    const Tuple first[] = {{1, 2}, {3, 4}, {5, 6}, {3, 4}};
    const Tuple second[] = {{3, 4}, {7, 8}};
    Tuple *result = NULL;
    size_t result_count = 0;

    if (remove_matching_tuples(
            first,
            sizeof(first) / sizeof(first[0]),
            second,
            sizeof(second) / sizeof(second[0]),
            &result,
            &result_count) != 0) {
        fputs("Failed to remove matching tuples.\n", stderr);
        return EXIT_FAILURE;
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