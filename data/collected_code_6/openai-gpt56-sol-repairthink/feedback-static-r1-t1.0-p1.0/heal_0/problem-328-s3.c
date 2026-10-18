#include <stddef.h>
#include <stdio.h>

static void reverse(int *array, size_t first, size_t last)
{
    while (first < last) {
        int temporary = array[first];
        array[first] = array[last];
        array[last] = temporary;
        ++first;
        --last;
    }
}

static int rotate_left(int *array, size_t length, size_t positions)
{
    if (array == NULL && length != 0U) {
        return -1;
    }

    if (length < 2U) {
        return 0;
    }

    positions %= length;

    if (positions == 0U) {
        return 0;
    }

    reverse(array, 0U, positions - 1U);
    reverse(array, positions, length - 1U);
    reverse(array, 0U, length - 1U);

    return 0;
}

int main(void)
{
    int values[] = {1, 2, 3, 4, 5, 6};
    const size_t length = sizeof values / sizeof values[0];
    const size_t positions = 2U;

    if (rotate_left(values, length, positions) != 0) {
        fputs("Rotation failed.\n", stderr);
        return 1;
    }

    for (size_t index = 0U; index < length; ++index) {
        if (printf("%d%s", values[index], index + 1U < length ? " " : "\n") < 0) {
            fputs("Output failed.\n", stderr);
            return 1;
        }
    }

    return 0;
}