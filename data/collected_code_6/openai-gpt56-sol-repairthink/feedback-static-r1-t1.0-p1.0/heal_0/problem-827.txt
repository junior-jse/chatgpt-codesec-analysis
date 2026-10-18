#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

int sum_column(const int *const *lists, const size_t *lengths,
               size_t row_count, size_t column, long long *result)
{
    size_t row;
    long long sum = 0;

    if (lists == NULL || lengths == NULL || result == NULL) {
        return -1;
    }

    for (row = 0; row < row_count; ++row) {
        if (lists[row] == NULL || column >= lengths[row]) {
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
    const size_t lengths[] = {
        sizeof row0 / sizeof row0[0],
        sizeof row1 / sizeof row1[0],
        sizeof row2 / sizeof row2[0]
    };
    long long result;

    if (sum_column(lists, lengths, sizeof lists / sizeof lists[0],
                   1U, &result) != 0) {
        fputs("Unable to sum the requested column.\n", stderr);
        return EXIT_FAILURE;
    }

    if (printf("%lld\n", result) < 0) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}