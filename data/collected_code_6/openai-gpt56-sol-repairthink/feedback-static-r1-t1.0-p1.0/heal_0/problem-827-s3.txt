#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

static int sum_column(const int *const *lists,
                      const size_t *column_counts,
                      size_t row_count,
                      size_t column,
                      long long *result)
{
    long long sum = 0;

    if (lists == NULL || column_counts == NULL || result == NULL) {
        return -1;
    }

    for (size_t row = 0; row < row_count; ++row) {
        if (lists[row] == NULL || column >= column_counts[row]) {
            return -1;
        }

        sum += lists[row][column];
    }

    *result = sum;
    return 0;
}

int main(void)
{
    static const int row0[] = {1, 2, 3};
    static const int row1[] = {4, 5, 6};
    static const int row2[] = {7, 8, 9};
    const int *lists[] = {row0, row1, row2};
    const size_t column_counts[] = {
        sizeof row0 / sizeof row0[0],
        sizeof row1 / sizeof row1[0],
        sizeof row2 / sizeof row2[0]
    };
    const size_t row_count = sizeof lists / sizeof lists[0];
    const size_t column = 1;
    long long sum = 0;

    if (sum_column(lists, column_counts, row_count, column, &sum) != 0) {
        fputs("Unable to sum the requested column.\n", stderr);
        return EXIT_FAILURE;
    }

    if (printf("%lld\n", sum) < 0) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}