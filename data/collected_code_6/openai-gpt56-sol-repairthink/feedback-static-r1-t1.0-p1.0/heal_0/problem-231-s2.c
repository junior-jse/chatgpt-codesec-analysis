#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <errno.h>

static int add_checked(int64_t a, int64_t b, int64_t *result)
{
    if ((b > 0 && a > INT64_MAX - b) ||
        (b < 0 && a < INT64_MIN - b)) {
        return 0;
    }

    *result = a + b;
    return 1;
}

static int maximum_triangle_sum(const int64_t *triangle, size_t rows,
                                int64_t *maximum)
{
    int64_t *sums = NULL;
    size_t element_count;
    size_t row;
    size_t column;
    size_t offset;

    if (triangle == NULL || maximum == NULL || rows == 0) {
        return 0;
    }

    if (rows > (SIZE_MAX - rows) / 2) {
        return 0;
    }

    element_count = rows * (rows + 1) / 2;

    if (element_count > SIZE_MAX / sizeof(*sums)) {
        return 0;
    }

    sums = malloc(rows * sizeof(*sums));
    if (sums == NULL) {
        return 0;
    }

    offset = element_count - rows;
    for (column = 0; column < rows; ++column) {
        sums[column] = triangle[offset + column];
    }

    for (row = rows - 1; row > 0; --row) {
        offset -= row;

        for (column = 0; column < row; ++column) {
            int64_t child = sums[column] > sums[column + 1]
                                ? sums[column]
                                : sums[column + 1];

            if (!add_checked(triangle[offset + column], child,
                             &sums[column])) {
                free(sums);
                return 0;
            }
        }
    }

    *maximum = sums[0];
    free(sums);
    return 1;
}

int main(void)
{
    size_t rows;
    size_t element_count;
    size_t index;
    int64_t *triangle = NULL;
    int64_t maximum;

    errno = 0;
    if (scanf("%zu", &rows) != 1 || errno == ERANGE || rows == 0) {
        fputs("Invalid row count\n", stderr);
        return EXIT_FAILURE;
    }

    if (rows > (SIZE_MAX - rows) / 2) {
        fputs("Triangle size is too large\n", stderr);
        return EXIT_FAILURE;
    }

    element_count = rows * (rows + 1) / 2;

    if (element_count > SIZE_MAX / sizeof(*triangle)) {
        fputs("Triangle size is too large\n", stderr);
        return EXIT_FAILURE;
    }

    triangle = malloc(element_count * sizeof(*triangle));
    if (triangle == NULL) {
        fputs("Memory allocation failed\n", stderr);
        return EXIT_FAILURE;
    }

    for (index = 0; index < element_count; ++index) {
        errno = 0;
        if (scanf("%" SCNd64, &triangle[index]) != 1 || errno == ERANGE) {
            fputs("Invalid triangle data\n", stderr);
            free(triangle);
            return EXIT_FAILURE;
        }
    }

    if (!maximum_triangle_sum(triangle, rows, &maximum)) {
        fputs("Unable to calculate maximum sum\n", stderr);
        free(triangle);
        return EXIT_FAILURE;
    }

    if (printf("%" PRId64 "\n", maximum) < 0) {
        fputs("Output error\n", stderr);
        free(triangle);
        return EXIT_FAILURE;
    }

    free(triangle);
    return EXIT_SUCCESS;
}