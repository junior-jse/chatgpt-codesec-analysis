#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *values;
    size_t length;
} Tuple;

static int trim_tuples(Tuple *tuples, size_t tuple_count, size_t k)
{
    if (tuples == NULL && tuple_count != 0) {
        return EINVAL;
    }

    for (size_t i = 0; i < tuple_count; ++i) {
        if (tuples[i].values == NULL && tuples[i].length != 0) {
            return EINVAL;
        }
        if (k > tuples[i].length) {
            return ERANGE;
        }
    }

    for (size_t i = 0; i < tuple_count; ++i) {
        tuples[i].length -= k;
    }

    return 0;
}

int main(void)
{
    int first_values[] = {1, 2, 3, 4};
    int second_values[] = {5, 6, 7};
    int third_values[] = {8, 9, 10, 11, 12};

    Tuple tuples[] = {
        {first_values, sizeof(first_values) / sizeof(first_values[0])},
        {second_values, sizeof(second_values) / sizeof(second_values[0])},
        {third_values, sizeof(third_values) / sizeof(third_values[0])}
    };

    const size_t tuple_count = sizeof(tuples) / sizeof(tuples[0]);
    const size_t k = 2;
    const int result = trim_tuples(tuples, tuple_count, k);

    if (result != 0) {
        errno = result;
        perror("trim_tuples");
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < tuple_count; ++i) {
        if (putchar('(') == EOF) {
            return EXIT_FAILURE;
        }

        for (size_t j = 0; j < tuples[i].length; ++j) {
            if (printf("%d%s", tuples[i].values[j],
                       j + 1 < tuples[i].length ? ", " : "") < 0) {
                return EXIT_FAILURE;
            }
        }

        if (puts(")") == EOF) {
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}