#include <stdio.h>
#include <stdlib.h>

static size_t maximum_sublist_length(const int *values, size_t count, int separator)
{
    size_t maximum = 0;
    size_t current = 0;

    if (values == NULL && count != 0) {
        return 0;
    }

    for (size_t i = 0; i < count; ++i) {
        if (values[i] == separator) {
            if (current > maximum) {
                maximum = current;
            }
            current = 0;
        } else {
            ++current;
        }
    }

    return current > maximum ? current : maximum;
}

int main(void)
{
    int values[] = {1, 2, 0, 3, 4, 5, 0, 6};
    size_t count = sizeof(values) / sizeof(values[0]);
    size_t maximum = maximum_sublist_length(values, count, 0);

    if (printf("%zu\n", maximum) < 0) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}