#include <stdio.h>
#include <stdlib.h>

typedef struct {
    const int *elements;
    size_t length;
} Sublist;

int *get_first_elements(const Sublist *sublists, size_t count)
{
    int *result;
    size_t i;

    if (count == 0) {
        return NULL;
    }

    if (sublists == NULL || count > SIZE_MAX / sizeof(*result)) {
        return NULL;
    }

    result = malloc(count * sizeof(*result));
    if (result == NULL) {
        return NULL;
    }

    for (i = 0; i < count; ++i) {
        if (sublists[i].elements == NULL || sublists[i].length == 0) {
            free(result);
            return NULL;
        }

        result[i] = sublists[i].elements[0];
    }

    return result;
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
    int *first_elements;
    size_t i;

    first_elements = get_first_elements(sublists, count);
    if (first_elements == NULL) {
        fputs("Failed to get first elements.\n", stderr);
        return EXIT_FAILURE;
    }

    for (i = 0; i < count; ++i) {
        if (printf("%d%s", first_elements[i], i + 1 < count ? " " : "\n") < 0) {
            free(first_elements);
            return EXIT_FAILURE;
        }
    }

    free(first_elements);
    return EXIT_SUCCESS;
}