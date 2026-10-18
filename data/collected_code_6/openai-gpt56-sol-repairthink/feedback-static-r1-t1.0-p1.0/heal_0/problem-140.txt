#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int first;
    int second;
} Tuple;

static int occurs_once(const Tuple *tuples, size_t count, int value)
{
    size_t occurrences = 0;

    for (size_t i = 0; i < count; ++i) {
        if (tuples[i].first == value) {
            ++occurrences;
        }
        if (tuples[i].second == value) {
            ++occurrences;
        }
        if (occurrences > 1) {
            return 0;
        }
    }

    return occurrences == 1;
}

static int extract_single_elements(const Tuple *tuples, size_t count,
                                   int **result, size_t *result_count)
{
    int *single_elements;
    size_t capacity;
    size_t used = 0;

    if (result == NULL || result_count == NULL ||
        (tuples == NULL && count != 0)) {
        return -1;
    }

    *result = NULL;
    *result_count = 0;

    if (count == 0) {
        return 0;
    }

    if (count > SIZE_MAX / 2 ||
        count * 2 > SIZE_MAX / sizeof(*single_elements)) {
        return -1;
    }

    capacity = count * 2;
    single_elements = malloc(capacity * sizeof(*single_elements));
    if (single_elements == NULL) {
        return -1;
    }

    for (size_t i = 0; i < count; ++i) {
        if (occurs_once(tuples, count, tuples[i].first)) {
            single_elements[used++] = tuples[i].first;
        }
        if (occurs_once(tuples, count, tuples[i].second)) {
            single_elements[used++] = tuples[i].second;
        }
    }

    if (used == 0) {
        free(single_elements);
        single_elements = NULL;
    } else {
        int *resized = realloc(single_elements, used * sizeof(*resized));
        if (resized != NULL) {
            single_elements = resized;
        }
    }

    *result = single_elements;
    *result_count = used;
    return 0;
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

    if (extract_single_elements(tuples, tuple_count,
                                &single_elements, &single_count) != 0) {
        fputs("Failed to extract elements.\n", stderr);
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < single_count; ++i) {
        if (printf("%d%s", single_elements[i],
                   i + 1 < single_count ? " " : "\n") < 0) {
            free(single_elements);
            return EXIT_FAILURE;
        }
    }

    if (single_count == 0 && putchar('\n') == EOF) {
        free(single_elements);
        return EXIT_FAILURE;
    }

    free(single_elements);
    return EXIT_SUCCESS;
}