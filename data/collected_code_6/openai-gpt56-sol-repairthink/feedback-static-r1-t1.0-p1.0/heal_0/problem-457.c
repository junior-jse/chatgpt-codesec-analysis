#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

typedef struct {
    int *values;
    size_t length;
} Sublist;

static int find_min_length_sublist(const Sublist *sublists,
                                   size_t count,
                                   size_t *result_index)
{
    if (sublists == NULL || result_index == NULL || count == 0U) {
        return -1;
    }

    size_t min_index = 0U;

    for (size_t i = 1U; i < count; ++i) {
        if (sublists[i].length < sublists[min_index].length) {
            min_index = i;
        }
    }

    *result_index = min_index;
    return 0;
}

int main(void)
{
    int first[] = {1, 2, 3, 4};
    int second[] = {5, 6};
    int third[] = {7, 8, 9};

    Sublist sublists[] = {
        {first, sizeof first / sizeof first[0]},
        {second, sizeof second / sizeof second[0]},
        {third, sizeof third / sizeof third[0]}
    };

    size_t count = sizeof sublists / sizeof sublists[0];
    size_t min_index = 0U;

    if (find_min_length_sublist(sublists, count, &min_index) != 0) {
        fputs("Unable to find a minimum-length sublist.\n", stderr);
        return EXIT_FAILURE;
    }

    printf("Minimum-length sublist: %zu\n", min_index);
    printf("Length: %zu\n", sublists[min_index].length);

    for (size_t i = 0U; i < sublists[min_index].length; ++i) {
        printf("%d%s",
               sublists[min_index].values[i],
               i + 1U == sublists[min_index].length ? "\n" : " ");
    }

    return EXIT_SUCCESS;
}