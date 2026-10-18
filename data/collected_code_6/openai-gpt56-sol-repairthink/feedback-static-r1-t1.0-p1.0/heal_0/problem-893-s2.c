#include <stdio.h>
#include <stdlib.h>

typedef struct {
    const int *elements;
    size_t length;
} Sublist;

int get_last_elements(const Sublist *sublists, size_t count, int **result)
{
    int *last_elements;
    size_t i;

    if (result == NULL || (count > 0 && sublists == NULL)) {
        return -1;
    }

    *result = NULL;

    if (count == 0) {
        return 0;
    }

    if (count > SIZE_MAX / sizeof(*last_elements)) {
        return -1;
    }

    for (i = 0; i < count; ++i) {
        if (sublists[i].elements == NULL || sublists[i].length == 0) {
            return -1;
        }
    }

    last_elements = malloc(count * sizeof(*last_elements));
    if (last_elements == NULL) {
        return -1;
    }

    for (i = 0; i < count; ++i) {
        last_elements[i] = sublists[i].elements[sublists[i].length - 1];
    }

    *result = last_elements;
    return 0;
}

int main(void)
{
    const int first[] = {1, 2, 3};
    const int second[] = {4, 5};
    const int third[] = {6, 7, 8, 9};
    const Sublist sublists[] = {
        {first, sizeof(first) / sizeof(first[0])},
        {second, sizeof(second) / sizeof(second[0])},
        {third, sizeof(third) / sizeof(third[0])}
    };
    const size_t count = sizeof(sublists) / sizeof(sublists[0]);
    int *last_elements = NULL;
    size_t i;

    if (get_last_elements(sublists, count, &last_elements) != 0) {
        fputs("Unable to get the last elements.\n", stderr);
        return EXIT_FAILURE;
    }

    for (i = 0; i < count; ++i) {
        if (printf("%d%s", last_elements[i], i + 1 == count ? "\n" : " ") < 0) {
            free(last_elements);
            return EXIT_FAILURE;
        }
    }

    free(last_elements);
    return EXIT_SUCCESS;
}