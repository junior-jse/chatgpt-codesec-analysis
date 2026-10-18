#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int generate_spiral_matrix(size_t n, uint64_t **matrix)
{
    size_t total;
    size_t top = 0;
    size_t left = 0;
    size_t bottom;
    size_t right;
    uint64_t value = 1;

    if (matrix == NULL || n == 0) {
        return -1;
    }

    if (n > SIZE_MAX / n) {
        return -1;
    }

    total = n * n;

    if (total > SIZE_MAX / sizeof(**matrix)) {
        return -1;
    }

    *matrix = malloc(total * sizeof(**matrix));
    if (*matrix == NULL) {
        return -1;
    }

    bottom = n - 1;
    right = n - 1;

    while (top <= bottom && left <= right) {
        for (size_t column = left; column <= right; ++column) {
            (*matrix)[top * n + column] = value++;
        }
        ++top;

        for (size_t row = top; row <= bottom; ++row) {
            (*matrix)[row * n + right] = value++;
        }

        if (right == 0) {
            break;
        }
        --right;

        if (top <= bottom) {
            for (size_t column = right + 1; column-- > left;) {
                (*matrix)[bottom * n + column] = value++;
            }

            if (bottom == 0) {
                break;
            }
            --bottom;
        }

        if (left <= right) {
            for (size_t row = bottom + 1; row-- > top;) {
                (*matrix)[row * n + left] = value++;
            }
            ++left;
        }
    }

    return 0;
}

int main(void)
{
    size_t n;
    uint64_t *matrix = NULL;

    if (scanf("%zu", &n) != 1 || n == 0) {
        fputs("Invalid matrix size\n", stderr);
        return EXIT_FAILURE;
    }

    if (generate_spiral_matrix(n, &matrix) != 0) {
        fputs("Unable to generate matrix\n", stderr);
        return EXIT_FAILURE;
    }

    for (size_t row = 0; row < n; ++row) {
        for (size_t column = 0; column < n; ++column) {
            if (printf("%llu%s",
                       (unsigned long long)matrix[row * n + column],
                       column + 1 == n ? "\n" : " ") < 0) {
                free(matrix);
                return EXIT_FAILURE;
            }
        }
    }

    free(matrix);
    return EXIT_SUCCESS;
}