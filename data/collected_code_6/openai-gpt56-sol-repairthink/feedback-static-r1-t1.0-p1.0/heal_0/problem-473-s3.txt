#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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

static int tuple_intersection(const Tuple *left,
                              size_t left_count,
                              const Tuple *right,
                              size_t right_count,
                              Tuple **result,
                              size_t *result_count)
{
    if (result == NULL || result_count == NULL ||
        (left == NULL && left_count != 0) ||
        (right == NULL && right_count != 0)) {
        return -1;
    }

    *result = NULL;
    *result_count = 0;

    if (left_count == 0 || right_count == 0) {
        return 0;
    }

    if (left_count > SIZE_MAX / sizeof(**result)) {
        return -1;
    }

    Tuple *intersection = malloc(left_count * sizeof(*intersection));
    if (intersection == NULL) {
        return -1;
    }

    size_t count = 0;

    for (size_t i = 0; i < left_count; ++i) {
        if (contains_tuple(right, right_count, &left[i]) &&
            !contains_tuple(intersection, count, &left[i])) {
            intersection[count++] = left[i];
        }
    }

    if (count == 0) {
        free(intersection);
        return 0;
    }

    Tuple *resized = realloc(intersection, count * sizeof(*intersection));
    if (resized != NULL) {
        intersection = resized;
    }

    *result = intersection;
    *result_count = count;
    return 0;
}

int main(void)
{
    const Tuple first_list[] = {
        {1, 2},
        {3, 4},
        {5, 6},
        {7, 8}
    };

    const Tuple second_list[] = {
        {6, 5},
        {4, 3},
        {9, 10}
    };

    Tuple *intersection = NULL;
    size_t intersection_count = 0;

    if (tuple_intersection(first_list,
                           sizeof(first_list) / sizeof(first_list[0]),
                           second_list,
                           sizeof(second_list) / sizeof(second_list[0]),
                           &intersection,
                           &intersection_count) != 0) {
        fputs("Failed to compute tuple intersection.\n", stderr);
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