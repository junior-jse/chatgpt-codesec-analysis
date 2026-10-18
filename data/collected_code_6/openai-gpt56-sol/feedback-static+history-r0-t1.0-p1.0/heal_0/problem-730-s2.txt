#include <stdio.h>
#include <stdlib.h>

static size_t remove_consecutive_duplicates(int *list, size_t length)
{
    if (list == NULL || length == 0U) {
        return 0U;
    }

    size_t write_index = 1U;

    for (size_t read_index = 1U; read_index < length; ++read_index) {
        if (list[read_index] != list[write_index - 1U]) {
            list[write_index] = list[read_index];
            ++write_index;
        }
    }

    return write_index;
}

int main(void)
{
    size_t length;

    if (scanf("%zu", &length) != 1) {
        fprintf(stderr, "Failed to read list length.\n");
        return EXIT_FAILURE;
    }

    if (length > SIZE_MAX / sizeof(int)) {
        fprintf(stderr, "List length is too large.\n");
        return EXIT_FAILURE;
    }

    int *list = NULL;

    if (length > 0U) {
        list = malloc(length * sizeof(*list));
        if (list == NULL) {
            fprintf(stderr, "Failed to allocate memory.\n");
            return EXIT_FAILURE;
        }
    }

    for (size_t i = 0U; i < length; ++i) {
        if (scanf("%d", &list[i]) != 1) {
            fprintf(stderr, "Failed to read list element.\n");
            free(list);
            return EXIT_FAILURE;
        }
    }

    const size_t new_length = remove_consecutive_duplicates(list, length);

    for (size_t i = 0U; i < new_length; ++i) {
        if (i > 0U && putchar(' ') == EOF) {
            free(list);
            return EXIT_FAILURE;
        }

        if (printf("%d", list[i]) < 0) {
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