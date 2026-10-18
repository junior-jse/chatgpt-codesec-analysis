#include <stdio.h>
#include <stdlib.h>

static size_t remove_negative_numbers(int *list, size_t length)
{
    size_t write_index = 0;

    if (list == NULL) {
        return 0;
    }

    for (size_t read_index = 0; read_index < length; ++read_index) {
        if (list[read_index] >= 0) {
            list[write_index++] = list[read_index];
        }
    }

    return write_index;
}

int main(void)
{
    int *list = NULL;
    size_t length = 0;

    if (scanf("%zu", &length) != 1) {
        fputs("Invalid list length\n", stderr);
        return EXIT_FAILURE;
    }

    if (length > SIZE_MAX / sizeof(*list)) {
        fputs("List is too large\n", stderr);
        return EXIT_FAILURE;
    }

    if (length > 0) {
        list = malloc(length * sizeof(*list));
        if (list == NULL) {
            fputs("Memory allocation failed\n", stderr);
            return EXIT_FAILURE;
        }
    }

    for (size_t i = 0; i < length; ++i) {
        if (scanf("%d", &list[i]) != 1) {
            fputs("Invalid list element\n", stderr);
            free(list);
            return EXIT_FAILURE;
        }
    }

    length = remove_negative_numbers(list, length);

    for (size_t i = 0; i < length; ++i) {
        if (i > 0) {
            putchar(' ');
        }
        printf("%d", list[i]);
    }
    putchar('\n');

    free(list);
    return EXIT_SUCCESS;
}