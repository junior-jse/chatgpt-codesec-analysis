#include <stdio.h>
#include <stdlib.h>

static size_t remove_odd_numbers(int *list, size_t length)
{
    size_t write_index = 0;

    if (list == NULL) {
        return 0;
    }

    for (size_t read_index = 0; read_index < length; ++read_index) {
        if (list[read_index] % 2 == 0) {
            list[write_index++] = list[read_index];
        }
    }

    return write_index;
}

int main(void)
{
    int list[] = {1, 2, 3, 4, 5, 6, 7, 8};
    size_t length = sizeof(list) / sizeof(list[0]);
    size_t new_length = remove_odd_numbers(list, length);

    for (size_t i = 0; i < new_length; ++i) {
        if (printf("%d%s", list[i], i + 1 < new_length ? " " : "\n") < 0) {
            return EXIT_FAILURE;
        }
    }

    if (new_length == 0 && putchar('\n') == EOF) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}