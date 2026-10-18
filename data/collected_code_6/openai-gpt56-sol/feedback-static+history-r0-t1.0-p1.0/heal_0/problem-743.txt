#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

static int rotate_right(int *items, size_t count, size_t positions)
{
    int *temporary;
    size_t shift;
    size_t i;

    if (items == NULL || count == 0U) {
        return -1;
    }

    shift = positions % count;
    if (shift == 0U) {
        return 0;
    }

    if (count > SIZE_MAX / sizeof(*temporary)) {
        return -1;
    }

    temporary = malloc(count * sizeof(*temporary));
    if (temporary == NULL) {
        return -1;
    }

    for (i = 0U; i < count; ++i) {
        temporary[(i + shift) % count] = items[i];
    }

    for (i = 0U; i < count; ++i) {
        items[i] = temporary[i];
    }

    free(temporary);
    return 0;
}

int main(void)
{
    int items[] = {1, 2, 3, 4, 5, 6};
    const size_t count = sizeof(items) / sizeof(items[0]);
    const size_t positions = 2U;
    size_t i;

    if (rotate_right(items, count, positions) != 0) {
        fprintf(stderr, "Failed to rotate the list.\n");
        return EXIT_FAILURE;
    }

    for (i = 0U; i < count; ++i) {
        if (printf("%d%s", items[i], (i + 1U < count) ? " " : "\n") < 0) {
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}