#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct {
    const int *items;
    size_t length;
} Sublist;

int find_min_length_sublist(const Sublist *sublists, size_t count, size_t *index)
{
    if (sublists == NULL || index == NULL || count == 0) {
        return -1;
    }

    size_t min_index = 0;

    for (size_t i = 0; i < count; ++i) {
        if (sublists[i].length > 0 && sublists[i].items == NULL) {
            return -1;
        }

        if (sublists[i].length < sublists[min_index].length) {
            min_index = i;
        }
    }

    *index = min_index;
    return 0;
}

int main(void)
{
    const int first[] = {1, 2, 3, 4};
    const int second[] = {5, 6};
    const int third[] = {7, 8, 9};

    const Sublist sublists[] = {
        {first, sizeof(first) / sizeof(first[0])},
        {second, sizeof(second) / sizeof(second[0])},
        {third, sizeof(third) / sizeof(third[0])}
    };

    const size_t count = sizeof(sublists) / sizeof(sublists[0]);
    size_t min_index = 0;

    if (find_min_length_sublist(sublists, count, &min_index) != 0) {
        fputs("Failed to find the minimum-length sublist.\n", stderr);
        return EXIT_FAILURE;
    }

    if (printf("Minimum-length sublist is at index %zu with length %zu\n",
               min_index, sublists[min_index].length) < 0) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}