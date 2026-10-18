#include <stdio.h>
#include <stdlib.h>

typedef struct {
    const int *elements;
    size_t length;
} Sublist;

size_t count_sublists_containing(const Sublist *sublists, size_t count, int target)
{
    size_t matches = 0;

    if (sublists == NULL && count != 0) {
        return 0;
    }

    for (size_t i = 0; i < count; ++i) {
        if (sublists[i].elements == NULL && sublists[i].length != 0) {
            continue;
        }

        for (size_t j = 0; j < sublists[i].length; ++j) {
            if (sublists[i].elements[j] == target) {
                ++matches;
                break;
            }
        }
    }

    return matches;
}

int main(void)
{
    const int first[] = {1, 2, 3};
    const int second[] = {4, 5, 6};
    const int third[] = {2, 7, 2};
    const int fourth[] = {8, 9};

    const Sublist sublists[] = {
        {first, sizeof first / sizeof first[0]},
        {second, sizeof second / sizeof second[0]},
        {third, sizeof third / sizeof third[0]},
        {fourth, sizeof fourth / sizeof fourth[0]}
    };

    int target = 0;

    if (printf("Enter the element to find: ") < 0) {
        return EXIT_FAILURE;
    }

    if (scanf("%d", &target) != 1) {
        fputs("Invalid input.\n", stderr);
        return EXIT_FAILURE;
    }

    const size_t count = count_sublists_containing(
        sublists,
        sizeof sublists / sizeof sublists[0],
        target
    );

    if (printf("%zu\n", count) < 0) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}