#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int first;
    int second;
} Tuple;

static int tuple_equal(const Tuple *a, const Tuple *b)
{
    return (a->first == b->first && a->second == b->second) ||
           (a->first == b->second && a->second == b->first);
}

static int contains_tuple(const Tuple *tuples, size_t count, const Tuple *target)
{
    size_t i;

    for (i = 0; i < count; ++i) {
        if (tuple_equal(&tuples[i], target)) {
            return 1;
        }
    }

    return 0;
}

static int tuple_intersection(const Tuple *first, size_t first_count,
                              const Tuple *second, size_t second_count,
                              Tuple **result, size_t *result_count)
{
    Tuple *intersection;
    size_t capacity;
    size_t count;
    size_t i;

    if (result == NULL || result_count == NULL ||
        (first == NULL && first_count != 0) ||
        (second == NULL && second_count != 0)) {
        return 0;
    }

    *result = NULL;
    *result_count = 0;

    capacity = first_count < second_count ? first_count : second_count;
    if (capacity == 0) {
        return 1;
    }

    if (capacity > SIZE_MAX / sizeof(*intersection)) {
        return 0;
    }

    intersection = malloc(capacity * sizeof(*intersection));
    if (intersection == NULL) {
        return 0;
    }

    count = 0;
    for (i = 0; i < first_count; ++i) {
        if (contains_tuple(second, second_count, &first[i]) &&
            !contains_tuple(intersection, count, &first[i])) {
            intersection[count++] = first[i];
        }
    }

    if (count == 0) {
        free(intersection);
        return 1;
    }

    *result = intersection;
    *result_count = count;
    return 1;
}

int main(void)
{
    Tuple first[] = {{1, 2}, {3, 4}, {5, 6}, {8, 7}};
    Tuple second[] = {{2, 1}, {4, 3}, {7, 8}, {9, 10}};
    Tuple *result = NULL;
    size_t result_count = 0;
    size_t i;

    if (!tuple_intersection(first, sizeof(first) / sizeof(first[0]),
                            second, sizeof(second) / sizeof(second[0]),
                            &result, &result_count)) {
        fputs("Failed to compute tuple intersection.\n", stderr);
        return EXIT_FAILURE;
    }

    for (i = 0; i < result_count; ++i) {
        if (printf("(%d, %d)\n", result[i].first, result[i].second) < 0) {
            free(result);
            return EXIT_FAILURE;
        }
    }

    free(result);
    return EXIT_SUCCESS;
}