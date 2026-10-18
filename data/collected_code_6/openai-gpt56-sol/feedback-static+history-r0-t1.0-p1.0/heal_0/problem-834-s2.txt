#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int generate_spiral(size_t n, uint64_t **matrix)
{
    size_t count;
    uint64_t *result;
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

    count = n * n;

    if (count > SIZE_MAX / sizeof(*result)) {
        return -1;
    }

    result = malloc(count * sizeof(*result));
    if (result == NULL) {
        return -1;
    }

    bottom = n - 1;
    right = n - 1;

    while (top <= bottom && left <= right) {
        size_t column;
        size_t row;

        for (column = left; column <= right; ++column) {
            result[top * n + column] = value * value;
            ++value;
        }
        ++top;

        for (row = top; row <= bottom; ++row) {
            result[row * n + right] = value * value;
            ++value;
        }

        if (right == 0) {
            break;
        }
        --right;

        if (top <= bottom) {
            column = right;
            for (;;) {
                result[bottom * n + column] = value * value;
                ++value;
                if (column == left) {
                    break;
                }
                --column;
            }

            if (bottom == 0) {
                break;
            }
            --bottom;
        }

        if (left <= right) {
            row = bottom;
            for (;;) {
                result[row * n + left] = value * value;
                ++value;
                if (row == top) {
                    break;
                }
                --row;
            }
            ++left;
        }
    }

    *matrix = result;
    return 0;
}

int main(int argc, char **argv)
{
    char *end;
    unsigned long long input;
    size_t n;
    uint64_t *matrix = NULL;
    size_t row;
    size_t column;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <positive matrix size>\n", argv[0]);
        return EXIT_FAILURE;
    }

    errno = 0;
    end = NULL;
    input = strtoull(argv[1], &end, 10);

    if (errno != 0 || end == argv[1] || *end != '\0' ||
        input == 0 || input > SIZE_MAX) {
        fprintf(stderr, "Invalid matrix size\n");
        return EXIT_FAILURE;
    }

    n = (size_t)input;

    if (n > UINT64_MAX / n) {
        fprintf(stderr, "Matrix values would overflow\n");
        return EXIT_FAILURE;
    }

    if (generate_spiral(n, &matrix) != 0) {
        fprintf(stderr, "Unable to generate matrix\n");
        return EXIT_FAILURE;
    }

    for (row = 0; row < n; ++row) {
        for (column = 0; column < n; ++column) {
            printf("%llu%s",
                   (unsigned long long)matrix[row * n + column],
                   column + 1 == n ? "\n" : " ");
        }
    }

    free(matrix);
    return EXIT_SUCCESS;
}