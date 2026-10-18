#include <stdio.h>
#include <stdlib.h>

static size_t count_in_range(const int *list, size_t length, int lower, int upper)
{
    size_t count = 0;

    if (list == NULL || lower > upper) {
        return 0;
    }

    for (size_t i = 0; i < length; ++i) {
        if (list[i] >= lower && list[i] <= upper) {
            ++count;
        }
    }

    return count;
}

int main(void)
{
    int list[] = {3, 7, 10, 15, 21, 25, 30};
    const size_t length = sizeof list / sizeof list[0];
    const int lower = 10;
    const int upper = 25;
    const size_t count = count_in_range(list, length, lower, upper);

    if (printf("%zu\n", count) < 0) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}