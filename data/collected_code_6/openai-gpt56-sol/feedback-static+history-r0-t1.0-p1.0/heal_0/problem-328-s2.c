#include <stdio.h>
#include <stdlib.h>

static int rotate_left(int *list, size_t length, size_t positions)
{
    int *temporary;
    size_t i;

    if (list == NULL || length == 0U) {
        return -1;
    }

    positions %= length;
    if (positions == 0U) {
        return 0;
    }

    temporary = malloc(positions * sizeof(*temporary));
    if (temporary == NULL) {
        return -1;
    }

    for (i = 0U; i < positions; ++i) {
        temporary[i] = list[i];
    }

    for (i = positions; i < length; ++i) {
        list[i - positions] = list[i];
    }

    for (i = 0U; i < positions; ++i) {
        list[length - positions + i] = temporary[i];
    }

    free(temporary);
    return 0;
}

int main(void)
{
    int list[] = {1, 2, 3, 4, 5, 6};
    const size_t length = sizeof(list) / sizeof(list[0]);
    const size_t positions = 2U;
    size_t i;

    if (rotate_left(list, length, positions) != 0) {
        fputs("Failed to rotate the list.\n", stderr);
        return EXIT_FAILURE;
    }

    for (i = 0U; i < length; ++i) {
        if (printf("%d%s", list[i], i + 1U == length ? "\n" : " ") < 0) {
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}