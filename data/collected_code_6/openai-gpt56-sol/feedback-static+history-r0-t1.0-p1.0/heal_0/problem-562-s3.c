#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

static int max_sublist_length(const int *values, size_t count, int delimiter,
                              size_t *result)
{
    size_t maximum = 0;
    size_t current = 0;

    if (result == NULL || (values == NULL && count != 0U)) {
        return -1;
    }

    for (size_t i = 0; i < count; ++i) {
        if (values[i] == delimiter) {
            if (current > maximum) {
                maximum = current;
            }
            current = 0;
        } else {
            ++current;
        }
    }

    if (current > maximum) {
        maximum = current;
    }

    *result = maximum;
    return 0;
}

int main(void)
{
    int values[] = {1, 2, 0, 3, 4, 5, 0, 6};
    const size_t count = sizeof(values) / sizeof(values[0]);
    size_t maximum = 0;

    if (max_sublist_length(values, count, 0, &maximum) != 0) {
        fputs("Failed to determine the maximum sublist length.\n", stderr);
        return EXIT_FAILURE;
    }

    if (printf("%zu\n", maximum) < 0) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}