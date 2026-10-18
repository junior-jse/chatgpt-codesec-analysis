#include <stdio.h>
#include <stdlib.h>

static size_t count_in_range(const int *values, size_t length, int lower, int upper)
{
    size_t count = 0;

    if (values == NULL || lower > upper) {
        return 0;
    }

    for (size_t i = 0; i < length; ++i) {
        if (values[i] >= lower && values[i] <= upper) {
            ++count;
        }
    }

    return count;
}

int main(void)
{
    const int values[] = {1, 4, 7, 10, 13, 16};
    const size_t length = sizeof(values) / sizeof(values[0]);
    const int lower = 4;
    const int upper = 13;
    const size_t count = count_in_range(values, length, lower, upper);

    if (printf("%zu\n", count) < 0) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}