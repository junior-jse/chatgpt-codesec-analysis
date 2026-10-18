#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

static void reverse(int *items, size_t first, size_t last)
{
    while (first < last) {
        int temporary = items[first];
        items[first] = items[last];
        items[last] = temporary;
        ++first;
        --last;
    }
}

static void rotate_left(int *items, size_t count, size_t positions)
{
    if (items == NULL || count < 2U) {
        return;
    }

    positions %= count;
    if (positions == 0U) {
        return;
    }

    reverse(items, 0U, positions - 1U);
    reverse(items, positions, count - 1U);
    reverse(items, 0U, count - 1U);
}

int main(void)
{
    int items[] = {1, 2, 3, 4, 5, 6};
    const size_t count = sizeof(items) / sizeof(items[0]);
    const size_t positions = 2U;

    rotate_left(items, count, positions);

    for (size_t index = 0U; index < count; ++index) {
        if (printf("%s%d", index == 0U ? "" : " ", items[index]) < 0) {
            return EXIT_FAILURE;
        }
    }

    if (putchar('\n') == EOF) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}