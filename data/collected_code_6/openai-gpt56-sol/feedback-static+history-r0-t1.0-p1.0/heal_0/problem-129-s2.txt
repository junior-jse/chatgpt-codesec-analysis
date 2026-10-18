#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

static int calculate_magic_square(size_t order, size_t *square)
{
    size_t row = 0;
    size_t column = order / 2;

    for (size_t value = 1; value <= order * order; ++value) {
        square[row * order + column] = value;

        size_t next_row = (row + order - 1) % order;
        size_t next_column = (column + 1) % order;

        if (square[next_row * order + next_column] != 0) {
            row = (row + 1) % order;
        } else {
            row = next_row;
            column = next_column;
        }
    }

    return 0;
}

static int parse_order(const char *text, size_t *order)
{
    char *end = NULL;
    unsigned long long value;

    if (text == NULL || order == NULL || *text == '\0') {
        return -1;
    }

    errno = 0;
    value = strtoull(text, &end, 10);

    if (errno != 0 || end == text || *end != '\0' ||
        value == 0 || (value % 2) == 0 || value > SIZE_MAX) {
        return -1;
    }

    *order = (size_t)value;
    return 0;
}

int main(int argc, char *argv[])
{
    size_t order;
    size_t count;
    size_t *square;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <positive-odd-order>\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (parse_order(argv[1], &order) != 0) {
        fprintf(stderr, "Invalid order\n");
        return EXIT_FAILURE;
    }

    if (order > SIZE_MAX / order) {
        fprintf(stderr, "Order is too large\n");
        return EXIT_FAILURE;
    }

    count = order * order;

    if (count > SIZE_MAX / sizeof(*square)) {
        fprintf(stderr, "Order is too large\n");
        return EXIT_FAILURE;
    }

    square = calloc(count, sizeof(*square));
    if (square == NULL) {
        perror("calloc");
        return EXIT_FAILURE;
    }

    if (calculate_magic_square(order, square) != 0) {
        free(square);
        return EXIT_FAILURE;
    }

    for (size_t row = 0; row < order; ++row) {
        for (size_t column = 0; column < order; ++column) {
            if (printf("%zu%s",
                       square[row * order + column],
                       column + 1 == order ? "\n" : "\t") < 0) {
                free(square);
                return EXIT_FAILURE;
            }
        }
    }

    free(square);

    if (fflush(stdout) == EOF) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}