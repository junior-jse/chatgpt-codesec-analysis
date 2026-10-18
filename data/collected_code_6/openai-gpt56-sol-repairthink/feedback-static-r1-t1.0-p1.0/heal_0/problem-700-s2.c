#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

static size_t count_in_range(const int *list, size_t length, int minimum, int maximum)
{
    size_t count = 0;

    if (list == NULL || minimum > maximum) {
        return 0;
    }

    for (size_t i = 0; i < length; ++i) {
        if (list[i] >= minimum && list[i] <= maximum) {
            ++count;
        }
    }

    return count;
}

int main(void)
{
    const int list[] = {3, 7, 12, 18, 21, 25, 30};
    const size_t length = sizeof(list) / sizeof(list[0]);
    const int minimum = 10;
    const int maximum = 25;
    const size_t count = count_in_range(list, length, minimum, maximum);

    if (printf("%zu\n", count) < 0) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}