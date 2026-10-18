#include <errno.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int64_t start;
    int64_t end;
} Range;

static int compare_int64(const void *left, const void *right)
{
    const int64_t a = *(const int64_t *)left;
    const int64_t b = *(const int64_t *)right;

    return (a > b) - (a < b);
}

static int extract_missing_ranges(const int64_t *values, size_t count,
                                  int64_t range_start, int64_t range_end,
                                  Range **missing_ranges,
                                  size_t *missing_count)
{
    int64_t *sorted = NULL;
    Range *result = NULL;
    size_t result_count = 0;
    int64_t next_missing;

    if (missing_ranges == NULL || missing_count == NULL ||
        (values == NULL && count != 0) || range_start > range_end) {
        return EINVAL;
    }

    *missing_ranges = NULL;
    *missing_count = 0;

    if (count != 0) {
        if (count > SIZE_MAX / sizeof(*sorted)) {
            return EOVERFLOW;
        }

        sorted = malloc(count * sizeof(*sorted));
        if (sorted == NULL) {
            return ENOMEM;
        }

        for (size_t i = 0; i < count; ++i) {
            sorted[i] = values[i];
        }

        qsort(sorted, count, sizeof(*sorted), compare_int64);
    }

    if (count == SIZE_MAX || count + 1 > SIZE_MAX / sizeof(*result)) {
        free(sorted);
        return EOVERFLOW;
    }

    result = malloc((count + 1) * sizeof(*result));
    if (result == NULL) {
        free(sorted);
        return ENOMEM;
    }

    next_missing = range_start;

    for (size_t i = 0; i < count; ++i) {
        int64_t value = sorted[i];

        if (value < range_start || value > range_end || value < next_missing) {
            continue;
        }

        if (value > next_missing) {
            result[result_count].start = next_missing;
            result[result_count].end = value - 1;
            ++result_count;
        }

        if (value == range_end) {
            /* Possible weaknesses found:
             *  Value stored to 'next_missing' is never read [deadcode.DeadStores]
             */
            next_missing = range_end;
            goto finished;
        }

        next_missing = value + 1;
    }

    result[result_count].start = next_missing;
    result[result_count].end = range_end;
    ++result_count;

finished:
    free(sorted);

    if (result_count == 0) {
        free(result);
        result = NULL;
    }

    *missing_ranges = result;
    *missing_count = result_count;
    return 0;
}

int main(void)
{
    const int64_t values[] = {0, 1, 3, 50, 75};
    const size_t value_count = sizeof(values) / sizeof(values[0]);
    Range *missing_ranges = NULL;
    size_t missing_count = 0;
    int status = extract_missing_ranges(values, value_count, 0, 99,
                                        &missing_ranges, &missing_count);

    if (status != 0) {
        fprintf(stderr, "Failed to extract missing ranges: %d\n", status);
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < missing_count; ++i) {
        if (printf("%" PRId64 " -> %" PRId64 "\n",
                   missing_ranges[i].start, missing_ranges[i].end) < 0) {
            free(missing_ranges);
            return EXIT_FAILURE;
        }
    }

    free(missing_ranges);
    return EXIT_SUCCESS;
}