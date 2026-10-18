#include <errno.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int64_t start;
    int64_t end;
} Range;

static int compare_int64(const void *lhs, const void *rhs)
{
    const int64_t a = *(const int64_t *)lhs;
    const int64_t b = *(const int64_t *)rhs;

    return (a > b) - (a < b);
}

static int extract_missing_ranges(int64_t *values, size_t count,
                                  int64_t range_start, int64_t range_end,
                                  Range **ranges_out, size_t *count_out)
{
    Range *ranges = NULL;
    size_t range_count = 0;
    int64_t next = range_start;

    if (ranges_out == NULL || count_out == NULL ||
        (count != 0 && values == NULL) || range_start > range_end) {
        return EINVAL;
    }

    *ranges_out = NULL;
    *count_out = 0;

    if (count > 1) {
        qsort(values, count, sizeof(*values), compare_int64);
    }

    if (count == SIZE_MAX || count + 1 > SIZE_MAX / sizeof(*ranges)) {
        return EOVERFLOW;
    }

    ranges = calloc(count + 1, sizeof(*ranges));
    if (ranges == NULL) {
        return ENOMEM;
    }

    for (size_t i = 0; i < count; ++i) {
        const int64_t value = values[i];

        if (value < next || value < range_start || value > range_end) {
            continue;
        }

        if (value > next) {
            ranges[range_count].start = next;
            ranges[range_count].end = value - 1;
            ++range_count;
        }

        if (value == range_end) {
            next = range_end;
            goto done;
        }

        next = value + 1;
    }

    if (next <= range_end) {
        ranges[range_count].start = next;
        ranges[range_count].end = range_end;
        ++range_count;
    }

done:
    if (range_count == 0) {
        free(ranges);
        ranges = NULL;
    }

    *ranges_out = ranges;
    *count_out = range_count;
    return 0;
}

int main(void)
{
    int64_t values[] = {0, 1, 3, 50, 75};
    const size_t value_count = sizeof(values) / sizeof(values[0]);
    const int64_t range_start = 0;
    const int64_t range_end = 99;
    Range *missing_ranges = NULL;
    size_t missing_count = 0;

    const int status = extract_missing_ranges(
        values,
        value_count,
        range_start,
        range_end,
        &missing_ranges,
        &missing_count
    );

    if (status != 0) {
        fprintf(stderr, "Failed to extract missing ranges: %d\n", status);
        free(missing_ranges);
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < missing_count; ++i) {
        if (missing_ranges[i].start == missing_ranges[i].end) {
            printf("%" PRId64 "\n", missing_ranges[i].start);
        } else {
            printf("%" PRId64 "->%" PRId64 "\n",
                   missing_ranges[i].start,
                   missing_ranges[i].end);
        }
    }

    free(missing_ranges);
    return EXIT_SUCCESS;
}