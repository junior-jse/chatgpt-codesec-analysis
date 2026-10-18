#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct List {
    int *values;
    size_t count;
} List;

static bool contains(const int *values, size_t count, int target)
{
    if (values == NULL) {
        return false;
    }

    for (size_t i = 0; i < count; ++i) {
        if (values[i] == target) {
            return true;
        }
    }

    return false;
}

static int find_nested_elements(
    const List *nested_lists,
    size_t nested_count,
    const int *other_list,
    size_t other_count,
    List **results_out)
{
    List *results = NULL;

    if (results_out == NULL ||
        (nested_count > 0U && nested_lists == NULL) ||
        (other_count > 0U && other_list == NULL)) {
        return -1;
    }

    *results_out = NULL;

    if (nested_count == 0U) {
        return 0;
    }

    if (nested_count > SIZE_MAX / sizeof(*results)) {
        return -1;
    }

    results = calloc(nested_count, sizeof(*results));
    if (results == NULL) {
        return -1;
    }

    for (size_t i = 0; i < nested_count; ++i) {
        const List *source = &nested_lists[i];

        if (source->count > 0U && source->values == NULL) {
            goto failure;
        }

        for (size_t j = 0; j < source->count; ++j) {
            int value = source->values[j];

            if (contains(other_list, other_count, value) &&
                !contains(results[i].values, results[i].count, value)) {
                int *resized;

                if (results[i].count == SIZE_MAX / sizeof(*results[i].values)) {
                    goto failure;
                }

                resized = realloc(
                    results[i].values,
                    (results[i].count + 1U) * sizeof(*results[i].values));
                if (resized == NULL) {
                    goto failure;
                }

                results[i].values = resized;
                results[i].values[results[i].count] = value;
                ++results[i].count;
            }
        }
    }

    *results_out = results;
    return 0;

failure:
    for (size_t i = 0; i < nested_count; ++i) {
        free(results[i].values);
    }
    free(results);
    return -1;
}

static void free_lists(List *lists, size_t count)
{
    if (lists == NULL) {
        return;
    }

    for (size_t i = 0; i < count; ++i) {
        free(lists[i].values);
    }

    free(lists);
}

int main(void)
{
    int first[] = {1, 2, 3, 4};
    int second[] = {3, 5, 6, 7};
    int third[] = {2, 4, 6, 8};
    int other[] = {2, 3, 6, 9};

    List nested[] = {
        {first, sizeof(first) / sizeof(first[0])},
        {second, sizeof(second) / sizeof(second[0])},
        {third, sizeof(third) / sizeof(third[0])}
    };

    const size_t nested_count = sizeof(nested) / sizeof(nested[0]);
    List *matches = NULL;

    if (find_nested_elements(
            nested,
            nested_count,
            other,
            sizeof(other) / sizeof(other[0]),
            &matches) != 0) {
        fputs("Unable to find matching elements.\n", stderr);
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < nested_count; ++i) {
        printf("List %zu:", i + 1U);
        for (size_t j = 0; j < matches[i].count; ++j) {
            printf(" %d", matches[i].values[j]);
        }
        putchar('\n');
    }

    free_lists(matches, nested_count);
    return EXIT_SUCCESS;
}