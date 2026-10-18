#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

static int generate_spiral_matrix(size_t n, unsigned long long **matrix)
{
    size_t count;
    size_t top = 0;
    size_t left = 0;
    size_t bottom;
    size_t right;
    unsigned long long value = 1;

    if (matrix == NULL || n == 0 || n > SIZE_MAX / n) {
        return -1;
    }

    count = n * n;
    if (count > SIZE_MAX / sizeof(**matrix)) {
        return -1;
    }

    *matrix = malloc(count * sizeof(**matrix));
    if (*matrix == NULL) {
        return -1;
    }

    bottom = n - 1;
    right = n - 1;

    while (top <= bottom && left <= right) {
        size_t i;

        for (i = left; i <= right; ++i) {
            (*matrix)[top * n + i] = value * value;
            ++value;
        }
        ++top;

        for (i = top; i <= bottom; ++i) {
            (*matrix)[i * n + right] = value * value;
            ++value;
        }

        if (right == 0) {
            break;
        }
        --right;

        if (top <= bottom) {
            for (i = right + 1; i-- > left;) {
                (*matrix)[bottom * n + i] = value * value;
                ++value;
            }

            if (bottom == 0) {
                break;
            }
            --bottom;
        }

        if (left <= right) {
            for (i = bottom + 1; i-- > top;) {
                (*matrix)[i * n + left] = value * value;
                ++value;
            }
            ++left;
        }
    }

    return 0;
}

static int print_matrix(const unsigned long long *matrix, size_t n)
{
    size_t row;
    size_t column;

    if (matrix == NULL || n == 0) {
        return -1;
    }

    for (row = 0; row < n; ++row) {
        for (column = 0; column < n; ++column) {
            if (printf("%llu%s",
                       matrix[row * n + column],
                       column + 1 == n ? "\n" : " ") < 0) {
                return -1;
            }
        }
    }

    return 0;
}

int main(void)
{
    char input[128];
    char *end;
    unsigned long long parsed;
    size_t n;
    unsigned long long *matrix = NULL;

    if (fgets(input, sizeof(input), stdin) == NULL) {
        fputs("Failed to read matrix size.\n", stderr);
        return EXIT_FAILURE;
    }

    errno = 0;
    end = input;
    parsed = strtoull(input, &end, 10);

    if (errno != 0 || end == input) {
        fputs("Invalid matrix size.\n", stderr);
        return EXIT_FAILURE;
    }

    while (*end == ' ' || *end == '\t' || *end == '\n' ||
           *end == '\r' || *end == '\f' || *end == '\v') {
        ++end;
    }

    if (*end != '\0' || parsed == 0 ||
        parsed > SIZE_MAX ||
        parsed > ULLONG_MAX / parsed) {
        fputs("Matrix size is out of range.\n", stderr);
        return EXIT_FAILURE;
    }

    n = (size_t)parsed;

    if (generate_spiral_matrix(n, &matrix) != 0) {
        fputs("Unable to generate matrix.\n", stderr);
        return EXIT_FAILURE;
    }

    if (print_matrix(matrix, n) != 0) {
        free(matrix);
        fputs("Unable to print matrix.\n", stderr);
        return EXIT_FAILURE;
    }

    free(matrix);
    return EXIT_SUCCESS;
}