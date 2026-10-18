#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int start;
    int end;
} Range;

static int compare_ints(const void *lhs, const void *rhs)
{
    const int a = *(const int *)lhs;
    const int b = *(const int *)rhs;

    return (a > b) - (a < b);
}

static int extract_missing_ranges(const int *values, size_t count,
                                  int range_start, int range_end,
                                  Range **result, size_t *result_count)
{
    int *sorted = NULL;
    Range *ranges = NULL;
    size_t capacity;
    size_t used = 0;
    long long next;

    if (result == NULL || result_count == NULL || range_start > range_end ||
        (count > 0 && values == NULL)) {
        return -1;
    }

    *result = NULL;
    *result_count = 0;

    if (count > SIZE_MAX / sizeof(*sorted)) {
        return -1;
    }

    if (count > 0) {
        sorted = malloc(count * sizeof(*sorted));
        if (sorted == NULL) {
            return -1;
        }

        for (size_t i = 0; i < count; ++i) {
            sorted[i] = values[i];
        }

        qsort(sorted, count, sizeof(*sorted), compare_ints);
    }

    if (count == SIZE_MAX) {
        free(sorted);
        return -1;
    }

    capacity = count + 1;
    if (capacity > SIZE_MAX / sizeof(*ranges)) {
        free(sorted);
        return -1;
    }

    ranges = malloc(capacity * sizeof(*ranges));
    if (ranges == NULL) {
        free(sorted);
        return -1;
    }

    next = range_start;

    for (size_t i = 0; i < count && next <= range_end; ++i) {
        long long value = sorted[i];

        if (value < next || value < range_start || value > range_end) {
            continue;
        }

        if (value > next) {
            ranges[used].start = (int)next;
            ranges[used].end = (int)(value - 1);
            ++used;
        }

        next = value + 1;
    }

    if (next <= range_end) {
        ranges[used].start = (int)next;
        ranges[used].end = range_end;
        ++used;
    }

    free(sorted);

    if (used == 0) {
        free(ranges);
        return 0;
    }

    *result = ranges;
    *result_count = used;
    return 0;
}

int main(void)
{
    const int values[] = {0, 1, 3, 50, 75};
    const size_t count = sizeof(values) / sizeof(values[0]);
    Range *missing = NULL;
    size_t missing_count = 0;

    if (extract_missing_ranges(values, count, 0, 99,
                               &missing, &missing_count) != 0) {
        fputs("Failed to extract missing ranges.\n", stderr);
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < missing_count; ++i) {
        if (missing[i].start == missing[i].end) {
            printf("%d\n", missing[i].start);
        } else {
            printf("%d-%d\n", missing[i].start, missing[i].end);
        }
    }

    free(missing);
    return EXIT_SUCCESS;
}