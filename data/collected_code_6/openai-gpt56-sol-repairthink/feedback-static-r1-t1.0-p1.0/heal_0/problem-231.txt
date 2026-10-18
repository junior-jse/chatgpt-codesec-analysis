#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <errno.h>

static int find_maximum_sum(int64_t **triangle, size_t rows, int64_t *result)
{
    if (triangle == NULL || result == NULL || rows == 0 || triangle[rows - 1] == NULL) {
        return -1;
    }

    int64_t *sums = malloc(rows * sizeof(*sums));
    if (sums == NULL) {
        return -1;
    }

    for (size_t i = 0; i < rows; ++i) {
        sums[i] = triangle[rows - 1][i];
    }

    for (size_t row = rows - 1; row > 0; --row) {
        if (triangle[row - 1] == NULL) {
            free(sums);
            return -1;
        }

        for (size_t col = 0; col < row; ++col) {
            int64_t best = sums[col] > sums[col + 1] ? sums[col] : sums[col + 1];

            if ((best > 0 && triangle[row - 1][col] > INT64_MAX - best) ||
                (best < 0 && triangle[row - 1][col] < INT64_MIN - best)) {
                free(sums);
                return -1;
            }

            sums[col] = triangle[row - 1][col] + best;
        }
    }

    *result = sums[0];
    free(sums);
    return 0;
}

int main(void)
{
    size_t rows;

    if (scanf("%zu", &rows) != 1 || rows == 0 ||
        rows > SIZE_MAX / sizeof(int64_t *)) {
        fputs("Invalid input\n", stderr);
        return EXIT_FAILURE;
    }

    int64_t **triangle = calloc(rows, sizeof(*triangle));
    if (triangle == NULL) {
        perror("calloc");
        return EXIT_FAILURE;
    }

    int status = EXIT_FAILURE;

    for (size_t row = 0; row < rows; ++row) {
        size_t count = row + 1;

        if (count > SIZE_MAX / sizeof(*triangle[row])) {
            fputs("Input is too large\n", stderr);
            goto cleanup;
        }

        triangle[row] = malloc(count * sizeof(*triangle[row]));
        if (triangle[row] == NULL) {
            perror("malloc");
            goto cleanup;
        }

        for (size_t col = 0; col < count; ++col) {
            if (scanf("%" SCNd64, &triangle[row][col]) != 1) {
                fputs("Invalid input\n", stderr);
                goto cleanup;
            }
        }
    }

    int64_t maximum_sum;
    if (find_maximum_sum(triangle, rows, &maximum_sum) != 0) {
        fputs("Unable to calculate maximum sum\n", stderr);
        goto cleanup;
    }

    if (printf("%" PRId64 "\n", maximum_sum) < 0) {
        perror("printf");
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