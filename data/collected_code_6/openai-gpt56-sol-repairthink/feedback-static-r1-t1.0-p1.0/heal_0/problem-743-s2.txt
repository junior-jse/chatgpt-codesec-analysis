#include <stdio.h>
#include <stdlib.h>

static int rotate_right(int *list, size_t length, size_t positions)
{
    if (list == NULL || length == 0) {
        return -1;
    }

    positions %= length;
    if (positions == 0) {
        return 0;
    }

    int *temporary = malloc(positions * sizeof(*temporary));
    if (temporary == NULL) {
        return -1;
    }

    for (size_t i = 0; i < positions; ++i) {
        temporary[i] = list[length - positions + i];
    }

    for (size_t i = length - positions; i > 0; --i) {
        list[i + positions - 1] = list[i - 1];
    }

    for (size_t i = 0; i < positions; ++i) {
        list[i] = temporary[i];
    }

    free(temporary);
    return 0;
}

int main(void)
{
    int list[] = {1, 2, 3, 4, 5, 6};
    const size_t length = sizeof(list) / sizeof(list[0]);
    const size_t positions = 2;

    if (rotate_right(list, length, positions) != 0) {
        fputs("Failed to rotate the list.\n", stderr);
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < length; ++i) {
        if (printf("%d%s", list[i], i + 1 == length ? "\n" : " ") < 0) {
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}