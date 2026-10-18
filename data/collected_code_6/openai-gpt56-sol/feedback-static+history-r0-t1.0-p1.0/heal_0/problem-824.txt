#include <stdio.h>
#include <stdlib.h>

static int remove_even_numbers(int *list, size_t *length)
{
    size_t read_index;
    size_t write_index = 0;

    if (list == NULL || length == NULL) {
        return -1;
    }

    for (read_index = 0; read_index < *length; ++read_index) {
        if (list[read_index] % 2 != 0) {
            list[write_index++] = list[read_index];
        }
    }

    *length = write_index;
    return 0;
}

int main(void)
{
    int *list = NULL;
    size_t length;
    size_t index;

    if (scanf("%zu", &length) != 1) {
        fputs("Invalid input\n", stderr);
        return EXIT_FAILURE;
    }

    if (length > SIZE_MAX / sizeof(*list)) {
        fputs("Input is too large\n", stderr);
        return EXIT_FAILURE;
    }

    if (length > 0) {
        list = malloc(length * sizeof(*list));
        if (list == NULL) {
            fputs("Memory allocation failed\n", stderr);
            return EXIT_FAILURE;
        }
    }

    for (index = 0; index < length; ++index) {
        if (scanf("%d", &list[index]) != 1) {
            fputs("Invalid input\n", stderr);
            free(list);
            return EXIT_FAILURE;
        }
    }

    if (remove_even_numbers(list, &length) != 0) {
        fputs("Failed to process the list\n", stderr);
        free(list);
        return EXIT_FAILURE;
    }

    for (index = 0; index < length; ++index) {
        if (index > 0 && putchar(' ') == EOF) {
            free(list);
            return EXIT_FAILURE;
        }

        if (printf("%d", list[index]) < 0) {
            free(list);
            return EXIT_FAILURE;
        }
    }

    if (putchar('\n') == EOF) {
        free(list);
        return EXIT_FAILURE;
    }

    free(list);
    return EXIT_SUCCESS;
}