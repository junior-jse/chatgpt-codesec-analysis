#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

static int compare_ints(const void *a, const void *b)
{
    const int left = *(const int *)a;
    const int right = *(const int *)b;

    return (left > right) - (left < right);
}

static int remove_two_duplicates(int *values, size_t *length)
{
    size_t read_index;
    size_t write_index = 0;
    size_t removed = 0;

    if (values == NULL || length == NULL || *length < 4U) {
        return -1;
    }

    qsort(values, *length, sizeof(*values), compare_ints);

    for (read_index = 0; read_index < *length;) {
        size_t end = read_index + 1U;

        while (end < *length && values[end] == values[read_index]) {
            ++end;
        }

        if (end - read_index > 1U && removed < 2U) {
            ++removed;
        } else {
            values[write_index++] = values[read_index];
        }

        read_index = end;
    }

    if (removed != 2U) {
        return -1;
    }

    *length = write_index;
    return 0;
}

static int read_size(size_t *value)
{
    unsigned long long input;

    if (value == NULL || scanf("%llu", &input) != 1) {
        return -1;
    }

    if (input == 0U || input > SIZE_MAX / sizeof(int)) {
        return -1;
    }

    *value = (size_t)input;
    return 0;
}

int main(void)
{
    size_t list_count;
    size_t list_index;

    if (read_size(&list_count) != 0) {
        fputs("Invalid number of lists.\n", stderr);
        return EXIT_FAILURE;
    }

    for (list_index = 0; list_index < list_count; ++list_index) {
        int *values = NULL;
        size_t length;
        size_t i;

        if (read_size(&length) != 0) {
            fputs("Invalid list length.\n", stderr);
            return EXIT_FAILURE;
        }

        values = malloc(length * sizeof(*values));
        if (values == NULL) {
            fputs("Memory allocation failed.\n", stderr);
            return EXIT_FAILURE;
        }

        for (i = 0; i < length; ++i) {
            if (scanf("%d", &values[i]) != 1) {
                fputs("Invalid list element.\n", stderr);
                free(values);
                return EXIT_FAILURE;
            }
        }

        if (remove_two_duplicates(values, &length) != 0) {
            fputs("Each list must contain at least two duplicated values.\n", stderr);
            free(values);
            return EXIT_FAILURE;
        }

        for (i = 0; i < length; ++i) {
            if (printf("%s%d", i == 0U ? "" : " ", values[i]) < 0) {
                free(values);
                return EXIT_FAILURE;
            }
        }

        if (putchar('\n') == EOF) {
            free(values);
            return EXIT_FAILURE;
        }

        free(values);
    }

    return EXIT_SUCCESS;
}