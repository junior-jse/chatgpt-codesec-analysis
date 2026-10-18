#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct {
    int *values;
    size_t length;
} IntList;

static int compare_ints(const void *a, const void *b)
{
    const int left = *(const int *)a;
    const int right = *(const int *)b;

    return (left > right) - (left < right);
}

static int remove_two_duplicates(IntList *lists, size_t list_count)
{
    size_t total = 0;
    size_t position = 0;
    int *combined = NULL;
    int duplicates[2];
    size_t duplicate_count = 0;

    if (lists == NULL && list_count != 0) {
        return -1;
    }

    for (size_t i = 0; i < list_count; ++i) {
        if (lists[i].values == NULL && lists[i].length != 0) {
            return -1;
        }
        if (lists[i].length > SIZE_MAX - total) {
            return -1;
        }
        total += lists[i].length;
    }

    if (total < 4 || total > SIZE_MAX / sizeof(*combined)) {
        return -1;
    }

    combined = malloc(total * sizeof(*combined));
    if (combined == NULL) {
        return -1;
    }

    for (size_t i = 0; i < list_count; ++i) {
        for (size_t j = 0; j < lists[i].length; ++j) {
            combined[position++] = lists[i].values[j];
        }
    }

    qsort(combined, total, sizeof(*combined), compare_ints);

    for (size_t i = 1; i < total && duplicate_count < 2; ++i) {
        if (combined[i] == combined[i - 1]) {
            if (duplicate_count == 0 ||
                duplicates[duplicate_count - 1] != combined[i]) {
                duplicates[duplicate_count++] = combined[i];
            }
        }
    }

    free(combined);

    if (duplicate_count != 2) {
        return -1;
    }

    for (size_t i = 0; i < list_count; ++i) {
        size_t write_index = 0;

        for (size_t read_index = 0; read_index < lists[i].length; ++read_index) {
            const int value = lists[i].values[read_index];

            if (value != duplicates[0] && value != duplicates[1]) {
                lists[i].values[write_index++] = value;
            }
        }

        lists[i].length = write_index;
    }

    return 0;
}

int main(void)
{
    int first[] = {1, 2, 3, 4};
    int second[] = {5, 2, 6};
    int third[] = {7, 8, 3, 9};
    IntList lists[] = {
        {first, sizeof(first) / sizeof(first[0])},
        {second, sizeof(second) / sizeof(second[0])},
        {third, sizeof(third) / sizeof(third[0])}
    };
    const size_t list_count = sizeof(lists) / sizeof(lists[0]);

    if (remove_two_duplicates(lists, list_count) != 0) {
        fputs("Unable to identify exactly two duplicate numbers.\n", stderr);
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < list_count; ++i) {
        for (size_t j = 0; j < lists[i].length; ++j) {
            if (printf("%d%s", lists[i].values[j],
                       j + 1 == lists[i].length ? "" : " ") < 0) {
                return EXIT_FAILURE;
            }
        }
        if (putchar('\n') == EOF) {
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}