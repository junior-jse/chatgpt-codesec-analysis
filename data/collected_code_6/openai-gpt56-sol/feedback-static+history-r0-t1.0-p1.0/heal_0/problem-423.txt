#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

static int64_t max3(int64_t a, int64_t b, int64_t c)
{
    int64_t result = a;

    if (b > result) {
        result = b;
    }
    if (c > result) {
        result = c;
    }

    return result;
}

static int gold_mine_max(const int64_t *mine, size_t rows, size_t cols,
                         int64_t *result)
{
    int64_t *next = NULL;
    int64_t *current = NULL;
    size_t row;
    size_t col;

    if (mine == NULL || result == NULL || rows == 0 || cols == 0) {
        return 0;
    }

    if (rows > SIZE_MAX / sizeof(*next)) {
        return 0;
    }

    next = malloc(rows * sizeof(*next));
    current = malloc(rows * sizeof(*current));

    if (next == NULL || current == NULL) {
        free(next);
        free(current);
        return 0;
    }

    for (row = 0; row < rows; ++row) {
        next[row] = mine[row * cols + (cols - 1)];
    }

    for (col = cols - 1; col > 0; --col) {
        size_t current_col = col - 1;

        for (row = 0; row < rows; ++row) {
            int64_t best = next[row];

            if (row > 0 && next[row - 1] > best) {
                best = next[row - 1];
            }
            if (row + 1 < rows && next[row + 1] > best) {
                best = next[row + 1];
            }

            if ((best > 0 && mine[row * cols + current_col] > INT64_MAX - best) ||
                (best < 0 && mine[row * cols + current_col] < INT64_MIN - best)) {
                free(next);
                free(current);
                return 0;
            }

            current[row] = mine[row * cols + current_col] + best;
        }

        {
            int64_t *temporary = next;
            next = current;
            current = temporary;
        }
    }

    *result = next[0];
    for (row = 1; row < rows; ++row) {
        *result = max3(*result, next[row], *result);
    }

    free(next);
    free(current);
    return 1;
}

int main(void)
{
    size_t rows;
    size_t cols;
    size_t count;
    size_t i;
    int64_t *mine = NULL;
    int64_t result;

    if (scanf("%zu %zu", &rows, &cols) != 2 || rows == 0 || cols == 0) {
        fputs("Invalid input\n", stderr);
        return EXIT_FAILURE;
    }

    if (rows > SIZE_MAX / cols) {
        fputs("Input is too large\n", stderr);
        return EXIT_FAILURE;
    }

    count = rows * cols;

    if (count > SIZE_MAX / sizeof(*mine)) {
        fputs("Input is too large\n", stderr);
        return EXIT_FAILURE;
    }

    mine = malloc(count * sizeof(*mine));
    if (mine == NULL) {
        fputs("Memory allocation failed\n", stderr);
        return EXIT_FAILURE;
    }

    for (i = 0; i < count; ++i) {
        if (scanf("%" SCNd64, &mine[i]) != 1) {
            fputs("Invalid input\n", stderr);
            free(mine);
            return EXIT_FAILURE;
        }
    }

    if (!gold_mine_max(mine, rows, cols, &result)) {
        fputs("Unable to compute result\n", stderr);
        free(mine);
        return EXIT_FAILURE;
    }

    if (printf("%" PRId64 "\n", result) < 0) {
        free(mine);
        return EXIT_FAILURE;
    }

    free(mine);
    return EXIT_SUCCESS;
}