#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct {
    int first;
    int second;
} Tuple;

static bool tuples_equal(const Tuple *left, const Tuple *right)
{
    return (left->first == right->first && left->second == right->second) ||
           (left->first == right->second && left->second == right->first);
}

static bool contains_tuple(const Tuple *tuples, size_t count, const Tuple *target)
{
    for (size_t i = 0; i < count; ++i) {
        if (tuples_equal(&tuples[i], target)) {
            return true;
        }
    }

    return false;
}

static bool tuple_intersection(const Tuple *first,
                               size_t first_count,
                               const Tuple *second,
                               size_t second_count,
                               Tuple **result,
                               size_t *result_count)
{
    if (result == NULL || result_count == NULL ||
        (first == NULL && first_count != 0U) ||
        (second == NULL && second_count != 0U)) {
        return false;
    }

    *result = NULL;
    *result_count = 0U;

    if (first_count == 0U || second_count == 0U) {
        return true;
    }

    if (first_count > SIZE_MAX / sizeof(Tuple)) {
        return false;
    }

    Tuple *intersection = malloc(first_count * sizeof(*intersection));
    if (intersection == NULL) {
        return false;
    }

    size_t count = 0U;

    for (size_t i = 0; i < first_count; ++i) {
        if (contains_tuple(second, second_count, &first[i]) &&
            !contains_tuple(intersection, count, &first[i])) {
            intersection[count++] = first[i];
        }
    }

    if (count == 0U) {
        free(intersection);
        return true;
    }

    Tuple *resized = realloc(intersection, count * sizeof(*intersection));
    if (resized != NULL) {
        intersection = resized;
    }

    *result = intersection;
    *result_count = count;
    return true;
}

int main(void)
{
    const Tuple first[] = {
        {1, 2},
        {3, 4},
        {5, 6},
        {8, 7}
    };

    const Tuple second[] = {
        {2, 1},
        {4, 3},
        {7, 8},
        {9, 10}
    };

    Tuple *intersection = NULL;
    size_t intersection_count = 0U;

    if (!tuple_intersection(first,
                            sizeof(first) / sizeof(first[0]),
                            second,
                            sizeof(second) / sizeof(second[0]),
                            &intersection,
                            &intersection_count)) {
        fputs("Unable to compute tuple intersection.\n", stderr);
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < intersection_count; ++i) {
        if (printf("(%d, %d)\n",
                   intersection[i].first,
                   intersection[i].second) < 0) {
            free(intersection);
            return EXIT_FAILURE;
        }
    }

    free(intersection);
    return EXIT_SUCCESS;
}