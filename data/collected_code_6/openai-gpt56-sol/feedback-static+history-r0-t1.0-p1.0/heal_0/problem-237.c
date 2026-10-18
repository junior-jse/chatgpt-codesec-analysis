#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

typedef struct {
    int64_t value;
    size_t count;
} RecordCount;

static int compare_int64(const void *lhs, const void *rhs)
{
    const int64_t a = *(const int64_t *)lhs;
    const int64_t b = *(const int64_t *)rhs;

    return (a > b) - (a < b);
}

static int find_similar_occurrences(const int64_t *records, size_t length,
                                    RecordCount **results, size_t *result_count)
{
    int64_t *sorted = NULL;
    RecordCount *counts = NULL;
    size_t unique_count = 0;
    size_t matched_count = 0;
    size_t i;

    if (results == NULL || result_count == NULL ||
        (records == NULL && length != 0)) {
        return -1;
    }

    *results = NULL;
    *result_count = 0;

    if (length == 0) {
        return 0;
    }

    if (length > SIZE_MAX / sizeof(*sorted)) {
        return -1;
    }

    sorted = malloc(length * sizeof(*sorted));
    if (sorted == NULL) {
        return -1;
    }

    for (i = 0; i < length; ++i) {
        sorted[i] = records[i];
    }

    qsort(sorted, length, sizeof(*sorted), compare_int64);

    for (i = 0; i < length;) {
        size_t j = i + 1;

        while (j < length && sorted[j] == sorted[i]) {
            ++j;
        }

        if (j - i > 1) {
            ++unique_count;
        }

        i = j;
    }

    if (unique_count == 0) {
        free(sorted);
        return 0;
    }

    if (unique_count > SIZE_MAX / sizeof(*counts)) {
        free(sorted);
        return -1;
    }

    counts = malloc(unique_count * sizeof(*counts));
    if (counts == NULL) {
        free(sorted);
        return -1;
    }

    for (i = 0; i < length;) {
        size_t j = i + 1;

        while (j < length && sorted[j] == sorted[i]) {
            ++j;
        }

        if (j - i > 1) {
            counts[matched_count].value = sorted[i];
            counts[matched_count].count = j - i;
            ++matched_count;
        }

        i = j;
    }

    free(sorted);
    *results = counts;
    *result_count = matched_count;
    return 0;
}

int main(void)
{
    size_t tuple_count;
    size_t total_records = 0;
    size_t offset = 0;
    int64_t *records = NULL;
    RecordCount *results = NULL;
    size_t result_count = 0;
    size_t i;

    if (scanf("%zu", &tuple_count) != 1) {
        return EXIT_FAILURE;
    }

    for (i = 0; i < tuple_count; ++i) {
        size_t tuple_size;
        size_t j;
        int64_t *resized;

        if (scanf("%zu", &tuple_size) != 1) {
            free(records);
            return EXIT_FAILURE;
        }

        if (tuple_size > SIZE_MAX - total_records ||
            total_records + tuple_size > SIZE_MAX / sizeof(*records)) {
            free(records);
            return EXIT_FAILURE;
        }

        if (tuple_size != 0) {
            resized = realloc(records,
                              (total_records + tuple_size) * sizeof(*records));
            if (resized == NULL) {
                free(records);
                return EXIT_FAILURE;
            }
            records = resized;
        }

        for (j = 0; j < tuple_size; ++j) {
            if (scanf("%" SCNd64, &records[offset + j]) != 1) {
                free(records);
                return EXIT_FAILURE;
            }
        }

        offset += tuple_size;
        total_records += tuple_size;
    }

    if (find_similar_occurrences(records, total_records,
                                 &results, &result_count) != 0) {
        free(records);
        return EXIT_FAILURE;
    }

    for (i = 0; i < result_count; ++i) {
        if (printf("%" PRId64 " %zu\n",
                   results[i].value, results[i].count) < 0) {
            free(results);
            free(records);
            return EXIT_FAILURE;
        }
    }

    free(results);
    free(records);
    return EXIT_SUCCESS;
}