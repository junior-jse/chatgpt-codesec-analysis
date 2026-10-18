#include <stdio.h>
#include <stdlib.h>

static int get_last_elements(const int * const *sublists,
                             const size_t *lengths,
                             size_t count,
                             int **result)
{
    int *last_elements;
    size_t i;

    if (sublists == NULL || lengths == NULL || result == NULL || count == 0) {
        return -1;
    }

    if (count > SIZE_MAX / sizeof(*last_elements)) {
        return -1;
    }

    last_elements = malloc(count * sizeof(*last_elements));
    if (last_elements == NULL) {
        return -1;
    }

    for (i = 0; i < count; ++i) {
        if (sublists[i] == NULL || lengths[i] == 0) {
            free(last_elements);
            return -1;
        }

        last_elements[i] = sublists[i][lengths[i] - 1];
    }

    *result = last_elements;
    return 0;
}

int main(void)
{
    const int first[] = {1, 2, 3};
    const int second[] = {4, 5};
    const int third[] = {6, 7, 8, 9};
    const int *sublists[] = {first, second, third};
    const size_t lengths[] = {
        sizeof(first) / sizeof(first[0]),
        sizeof(second) / sizeof(second[0]),
        sizeof(third) / sizeof(third[0])
    };
    const size_t count = sizeof(sublists) / sizeof(sublists[0]);
    int *last_elements = NULL;
    size_t i;

    if (get_last_elements(sublists, lengths, count, &last_elements) != 0) {
        fputs("Failed to get last elements.\n", stderr);
        return EXIT_FAILURE;
    }

    for (i = 0; i < count; ++i) {
        if (printf("%d%s", last_elements[i], i + 1 < count ? " " : "\n") < 0) {
            free(last_elements);
            return EXIT_FAILURE;
        }
    }

    free(last_elements);
    return EXIT_SUCCESS;
}