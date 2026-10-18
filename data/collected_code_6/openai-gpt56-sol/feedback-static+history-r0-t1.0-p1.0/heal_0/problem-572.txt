#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

static int compare_ints(const void *left, const void *right)
{
    const int a = *(const int *)left;
    const int b = *(const int *)right;

    return (a > b) - (a < b);
}

static int remove_two_duplicates(int *values, size_t *count)
{
    size_t read_index;
    size_t write_index;
    size_t removed;

    if (values == NULL || count == NULL || *count < 2U) {
        return 0;
    }

    qsort(values, *count, sizeof(*values), compare_ints);

    write_index = 1U;
    removed = 0U;

    for (read_index = 1U; read_index < *count; ++read_index) {
        if (removed < 2U && values[read_index] == values[write_index - 1U]) {
            ++removed;
        } else {
            values[write_index] = values[read_index];
            ++write_index;
        }
    }

    if (removed != 2U) {
        return 0;
    }

    *count = write_index;
    return 1;
}

int main(void)
{
    size_t list_count;
    size_t list_index;

    if (scanf("%zu", &list_count) != 1) {
        return EXIT_FAILURE;
    }

    for (list_index = 0U; list_index < list_count; ++list_index) {
        size_t count;
        size_t index;
        int *values;

        if (scanf("%zu", &count) != 1 || count == 0U ||
            count > SIZE_MAX / sizeof(*values)) {
            return EXIT_FAILURE;
        }

        values = malloc(count * sizeof(*values));
        if (values == NULL) {
            return EXIT_FAILURE;
        }

        for (index = 0U; index < count; ++index) {
            if (scanf("%d", &values[index]) != 1) {
                free(values);
                return EXIT_FAILURE;
            }
        }

        if (!remove_two_duplicates(values, &count)) {
            free(values);
            return EXIT_FAILURE;
        }

        for (index = 0U; index < count; ++index) {
            if (printf("%s%d", index == 0U ? "" : " ", values[index]) < 0) {
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