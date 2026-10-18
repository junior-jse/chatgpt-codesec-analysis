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

    if (mine == NULL || result == NULL || rows == 0U || cols == 0U) {
        return -1;
    }

    if (rows > SIZE_MAX / sizeof(*next)) {
        return -1;
    }

    next = malloc(rows * sizeof(*next));
    current = malloc(rows * sizeof(*current));
    if (next == NULL || current == NULL) {
        free(next);
        free(current);
        return -1;
    }

    for (size_t row = 0U; row < rows; ++row) {
        next[row] = mine[row * cols + (cols - 1U)];
    }

    for (size_t col = cols - 1U; col > 0U; --col) {
        size_t current_col = col - 1U;

        for (size_t row = 0U; row < rows; ++row) {
            int64_t right = next[row];
            int64_t right_up = row > 0U ? next[row - 1U] : INT64_MIN;
            int64_t right_down = row + 1U < rows ? next[row + 1U] : INT64_MIN;
            int64_t best = max3(right, right_up, right_down);
            int64_t value = mine[row * cols + current_col];

            if ((best > 0 && value > INT64_MAX - best) ||
                (best < 0 && value < INT64_MIN - best)) {
                free(next);
                free(current);
                return -1;
            }

            current[row] = value + best;
        }

        int64_t *temporary = next;
        next = current;
        current = temporary;
    }

    *result = next[0];
    for (size_t row = 1U; row < rows; ++row) {
        if (next[row] > *result) {
            *result = next[row];
        }
    }

    free(next);
    free(current);
    return 0;
}

int main(void)
{
    size_t rows;
    size_t cols;
    int64_t *mine = NULL;
    int64_t result;

    if (scanf("%zu %zu", &rows, &cols) != 2 || rows == 0U || cols == 0U) {
        fputs("Invalid dimensions\n", stderr);
        return EXIT_FAILURE;
    }

    if (rows > SIZE_MAX / cols ||
        rows * cols > SIZE_MAX / sizeof(*mine)) {
        fputs("Dimensions are too large\n", stderr);
        return EXIT_FAILURE;
    }

    mine = malloc(rows * cols * sizeof(*mine));
    if (mine == NULL) {
        fputs("Memory allocation failed\n", stderr);
        return EXIT_FAILURE;
    }

    for (size_t row = 0U; row < rows; ++row) {
        for (size_t col = 0U; col < cols; ++col) {
            if (scanf("%" SCNd64, &mine[row * cols + col]) != 1) {
                fputs("Invalid mine data\n", stderr);
                free(mine);
                return EXIT_FAILURE;
            }
        }
    }

    if (gold_mine_max(mine, rows, cols, &result) != 0) {
        fputs("Unable to compute result\n", stderr);
        free(mine);
        return EXIT_FAILURE;
    }

    printf("%" PRId64 "\n", result);

    free(mine);
    return EXIT_SUCCESS;
}