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

static int solve_gold_mine(const int64_t *mine, size_t rows, size_t cols,
                           int64_t *result)
{
    int64_t *next = NULL;
    int64_t *current = NULL;

    if (mine == NULL || result == NULL || rows == 0 || cols == 0) {
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

    for (size_t row = 0; row < rows; ++row) {
        next[row] = mine[row * cols + (cols - 1)];
    }

    for (size_t col = cols - 1; col > 0; --col) {
        size_t previous_col = col - 1;

        for (size_t row = 0; row < rows; ++row) {
            int64_t right = next[row];
            int64_t right_up = row > 0 ? next[row - 1] : INT64_MIN;
            int64_t right_down = row + 1 < rows ? next[row + 1] : INT64_MIN;
            int64_t best = max3(right, right_up, right_down);
            int64_t value = mine[row * cols + previous_col];

            if ((best > 0 && value > INT64_MAX - best) ||
                (best < 0 && value < INT64_MIN - best)) {
                free(current);
                free(next);
                return -1;
            }

            current[row] = value + best;
        }

        int64_t *temporary = next;
        next = current;
        current = temporary;
    }

    *result = next[0];
    for (size_t row = 1; row < rows; ++row) {
        if (next[row] > *result) {
            *result = next[row];
        }
    }

    free(current);
    free(next);
    return 0;
}

int main(void)
{
    size_t rows;
    size_t cols;
    int64_t *mine = NULL;
    int64_t result;

    if (scanf("%zu %zu", &rows, &cols) != 2 || rows == 0 || cols == 0) {
        fputs("Invalid input\n", stderr);
        return EXIT_FAILURE;
    }

    if (rows > SIZE_MAX / cols ||
        rows * cols > SIZE_MAX / sizeof(*mine)) {
        fputs("Input is too large\n", stderr);
        return EXIT_FAILURE;
    }

    mine = malloc(rows * cols * sizeof(*mine));
    if (mine == NULL) {
        fputs("Memory allocation failed\n", stderr);
        return EXIT_FAILURE;
    }

    for (size_t row = 0; row < rows; ++row) {
        for (size_t col = 0; col < cols; ++col) {
            /* Possible weaknesses found:
             * Flawfinder scanf: The scanf() family's %s operation, without a limit specification, permits buffer overflows (CWE-120, CWE-20). Specify a limit to %s, or use a different input function. If the scanf format is influenceable by an attacker, it's exploitable. (risk 4, buffer)
             */
            if (scanf("%" SCNd64, &mine[row * cols + col]) != 1) {
                fputs("Invalid input\n", stderr);
                free(mine);
                return EXIT_FAILURE;
            }
        }
    }

    if (solve_gold_mine(mine, rows, cols, &result) != 0) {
        fputs("Unable to solve gold mine\n", stderr);
        free(mine);
        return EXIT_FAILURE;
    }

    printf("%" PRId64 "\n", result);
    free(mine);
    return EXIT_SUCCESS;
}