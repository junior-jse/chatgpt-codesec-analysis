#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

static int checked_add(long long a, long long b, long long *result)
{
    if ((b > 0 && a > LLONG_MAX - b) ||
        (b < 0 && a < LLONG_MIN - b)) {
        return 0;
    }

    *result = a + b;
    return 1;
}

static int maximum_triangle_sum(long long **triangle, size_t rows,
                                long long *result)
{
    long long *sums = NULL;

    if (triangle == NULL || result == NULL || rows == 0 ||
        rows > SIZE_MAX / sizeof(*sums)) {
        return 0;
    }

    sums = malloc(rows * sizeof(*sums));
    if (sums == NULL) {
        return 0;
    }

    for (size_t column = 0; column < rows; ++column) {
        sums[column] = triangle[rows - 1][column];
    }

    for (size_t row = rows - 1; row > 0; --row) {
        for (size_t column = 0; column < row; ++column) {
            long long child = sums[column] > sums[column + 1]
                                  ? sums[column]
                                  : sums[column + 1];

            if (!checked_add(triangle[row - 1][column], child,
                             &sums[column])) {
                free(sums);
                return 0;
            }
        }
    }

    *result = sums[0];
    free(sums);
    return 1;
}

static int read_long_long(long long *value)
{
    return scanf("%lld", value) == 1;
}

int main(void)
{
    size_t rows = 0;
    long long rows_input = 0;
    long long **triangle = NULL;
    long long maximum = 0;
    int status = EXIT_FAILURE;

    if (!read_long_long(&rows_input) || rows_input <= 0 ||
        (unsigned long long)rows_input > SIZE_MAX / sizeof(*triangle)) {
        fprintf(stderr, "Invalid row count\n");
        return EXIT_FAILURE;
    }

    rows = (size_t)rows_input;
    triangle = calloc(rows, sizeof(*triangle));
    if (triangle == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    for (size_t row = 0; row < rows; ++row) {
        if (row + 1 > SIZE_MAX / sizeof(*triangle[row])) {
            fprintf(stderr, "Triangle is too large\n");
            goto cleanup;
        }

        triangle[row] = malloc((row + 1) * sizeof(*triangle[row]));
        if (triangle[row] == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            goto cleanup;
        }

        for (size_t column = 0; column <= row; ++column) {
            if (!read_long_long(&triangle[row][column])) {
                fprintf(stderr, "Invalid triangle value\n");
                goto cleanup;
            }
        }
    }

    if (!maximum_triangle_sum(triangle, rows, &maximum)) {
        fprintf(stderr, "Unable to compute maximum sum\n");
        goto cleanup;
    }

    if (printf("%lld\n", maximum) < 0) {
        fprintf(stderr, "Output error\n");
        goto cleanup;
    }

    status = EXIT_SUCCESS;

cleanup:
    for (size_t row = 0; row < rows; ++row) {
        free(triangle[row]);
    }
    free(triangle);

    return status;
}