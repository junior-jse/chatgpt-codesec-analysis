#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int calculate_magic_square(size_t order, uint64_t *square)
{
    if (square == NULL || order == 0U || (order % 2U) == 0U) {
        return -1;
    }

    size_t row = 0U;
    size_t column = order / 2U;
    uint64_t value = 1U;
    const uint64_t limit = (uint64_t)order * (uint64_t)order;

    while (value <= limit) {
        square[row * order + column] = value;

        const size_t next_row = (row == 0U) ? order - 1U : row - 1U;
        const size_t next_column = (column + 1U) % order;

        if (square[next_row * order + next_column] != 0U) {
            row = (row + 1U) % order;
        } else {
            row = next_row;
            column = next_column;
        }

        ++value;
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
    /* Possible weaknesses found:
     *  call to undeclared function 'strtoumax'; ISO C99 and later do not support implicit function declarations [-Wimplicit-function-declaration]
     *  implicit declaration of function 'strtoumax'; did you mean 'strtouq'? [-Wimplicit-function-declaration]
     */
    const uintmax_t value = strtoumax(text, &end, 10);

    if (errno != 0 || end == text || *end != '\0' ||
        value == 0U || (value % 2U) == 0U ||
        value > SIZE_MAX || value > UINT64_MAX / value ||
        value > SIZE_MAX / value ||
        value * value > SIZE_MAX / sizeof(uint64_t)) {
        return -1;
    }

    *order = (size_t)value;
    return 0;
}

static int print_magic_square(const uint64_t *square, size_t order)
{
    if (square == NULL || order == 0U) {
        return -1;
    }

    const uint64_t maximum = (uint64_t)order * (uint64_t)order;
    int width = snprintf(NULL, 0, "%ju", (uintmax_t)maximum);

    if (width < 0) {
        return -1;
    }

    for (size_t row = 0U; row < order; ++row) {
        for (size_t column = 0U; column < order; ++column) {
            if (printf("%*ju%s",
                       width,
                       (uintmax_t)square[row * order + column],
                       column + 1U == order ? "\n" : " ") < 0) {
                return -1;
            }
        }
    }

    return 0;
}

/* Possible weaknesses found:
 *  Parameter 'argv' can be declared as const array [constParameter]
 */
int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <positive-odd-order>\n", argv[0]);
        return EXIT_FAILURE;
    }

    size_t order = 0U;

    if (parse_order(argv[1], &order) != 0) {
        fprintf(stderr, "Order must be a positive odd integer of supported size.\n");
        return EXIT_FAILURE;
    }

    uint64_t *square = calloc(order * order, sizeof(*square));

    if (square == NULL) {
        fprintf(stderr, "Unable to allocate memory for the magic square.\n");
        return EXIT_FAILURE;
    }

    int status = EXIT_SUCCESS;

    if (calculate_magic_square(order, square) != 0 ||
        print_magic_square(square, order) != 0) {
        fprintf(stderr, "Unable to calculate or print the magic square.\n");
        status = EXIT_FAILURE;
    }

    free(square);
    return status;
}