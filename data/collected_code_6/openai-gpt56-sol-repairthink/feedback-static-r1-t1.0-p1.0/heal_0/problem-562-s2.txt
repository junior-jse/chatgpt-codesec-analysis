#include <stdio.h>
#include <stdlib.h>

static int max_sublist_length(const int *values, size_t count, int separator, size_t *result)
{
    size_t current_length = 0;
    size_t maximum_length = 0;

    if (result == NULL || (values == NULL && count != 0)) {
        return -1;
    }

    for (size_t i = 0; i < count; ++i) {
        if (values[i] == separator) {
            if (current_length > maximum_length) {
                maximum_length = current_length;
            }
            current_length = 0;
        } else {
            ++current_length;
        }
    }

    if (current_length > maximum_length) {
        maximum_length = current_length;
    }

    *result = maximum_length;
    return 0;
}

int main(void)
{
    const int values[] = {1, 2, 0, 3, 4, 5, 0, 6, 7};
    const size_t count = sizeof(values) / sizeof(values[0]);
    size_t maximum_length = 0;

    if (max_sublist_length(values, count, 0, &maximum_length) != 0) {
        fputs("Failed to compute the maximum sublist length.\n", stderr);
        return EXIT_FAILURE;
    }

    if (printf("%zu\n", maximum_length) < 0) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}