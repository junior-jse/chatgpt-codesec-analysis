#include <stdio.h>
#include <stdlib.h>

static int remove_odd_numbers(int *list, size_t *length)
{
    if (list == NULL || length == NULL) {
        return -1;
    }

    size_t write_index = 0;

    for (size_t read_index = 0; read_index < *length; ++read_index) {
        if (list[read_index] % 2 == 0) {
            list[write_index++] = list[read_index];
        }
    }

    *length = write_index;
    return 0;
}

int main(void)
{
    int list[] = {1, 2, 3, 4, 5, 6, 7, 8};
    size_t length = sizeof(list) / sizeof(list[0]);

    if (remove_odd_numbers(list, &length) != 0) {
        fputs("Failed to remove odd numbers.\n", stderr);
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < length; ++i) {
        if (i > 0 && putchar(' ') == EOF) {
            return EXIT_FAILURE;
        }

        if (printf("%d", list[i]) < 0) {
            return EXIT_FAILURE;
        }
    }

    if (putchar('\n') == EOF) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}