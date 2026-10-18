#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int calculate_magic_square(size_t order, size_t *square)
{
    if (square == NULL || order == 0U || order % 2U == 0U) {
        return -1;
    }

    size_t row = 0U;
    size_t column = order / 2U;
    size_t count = order * order;

    for (size_t value = 1U; value <= count; ++value) {
        square[row * order + column] = value;

        size_t next_row = (row == 0U) ? order - 1U : row - 1U;
        size_t next_column = (column + 1U) % order;

        if (square[next_row * order + next_column] != 0U) {
            row = (row + 1U) % order;
        } else {
            row = next_row;
            column = next_column;
        }
    }

    return 0;
}

static int parse_order(const char *text, size_t *order)
{
    if (text == NULL || order == NULL || *text == '\0' || *text == '-') {
        return -1;
    }

    errno = 0;
    char *end = NULL;
    unsigned long long value = strtoull(text, &end, 10);

    if (errno == ERANGE || end == text || *end != '\0' ||
        value == 0ULL || value > SIZE_MAX || value % 2ULL == 0ULL) {
        return -1;
    }

    *order = (size_t)value;
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <positive-odd-order>\n", argv[0]);
        return EXIT_FAILURE;
    }

    size_t order = 0U;

    if (parse_order(argv[1], &order) != 0) {
        fputs("Order must be a positive odd integer.\n", stderr);
        return EXIT_FAILURE;
    }

    if (order > SIZE_MAX / order) {
        fputs("Order is too large.\n", stderr);
        return EXIT_FAILURE;
    }

    size_t count = order * order;

    if (count > SIZE_MAX / sizeof(size_t)) {
        fputs("Order is too large.\n", stderr);
        return EXIT_FAILURE;
    }

    size_t *square = calloc(count, sizeof(*square));

    if (square == NULL) {
        perror("calloc");
        return EXIT_FAILURE;
    }

    if (calculate_magic_square(order, square) != 0) {
        fputs("Failed to calculate the magic square.\n", stderr);
        free(square);
        return EXIT_FAILURE;
    }

    size_t maximum = count;
    int width = 1;

    while (maximum >= 10U) {
        maximum /= 10U;
        ++width;
    }

    for (size_t row = 0U; row < order; ++row) {
        for (size_t column = 0U; column < order; ++column) {
            if (printf("%*zu%s",
                       width,
                       square[row * order + column],
                       column + 1U == order ? "\n" : " ") < 0) {
                fputs("Output error.\n", stderr);
                free(square);
                return EXIT_FAILURE;
            }
        }
    }

    free(square);
    return EXIT_SUCCESS;
}