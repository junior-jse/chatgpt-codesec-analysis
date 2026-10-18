#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

static int remove_odd_numbers(int *numbers, size_t *length)
{
    size_t read_index;
    size_t write_index = 0;

    if (numbers == NULL || length == NULL) {
        return -1;
    }

    for (read_index = 0; read_index < *length; ++read_index) {
        if (numbers[read_index] % 2 == 0) {
            numbers[write_index++] = numbers[read_index];
        }
    }

    *length = write_index;
    return 0;
}

int main(void)
{
    int numbers[] = {1, 2, 3, 4, 5, 6, 7, 8};
    size_t length = sizeof(numbers) / sizeof(numbers[0]);
    size_t index;

    if (remove_odd_numbers(numbers, &length) != 0) {
        fputs("Failed to remove odd numbers.\n", stderr);
        return EXIT_FAILURE;
    }

    for (index = 0; index < length; ++index) {
        if (printf("%s%d", index == 0 ? "" : " ", numbers[index]) < 0) {
            return EXIT_FAILURE;
        }
    }

    if (putchar('\n') == EOF) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}