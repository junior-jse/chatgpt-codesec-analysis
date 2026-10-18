#include <stdio.h>
#include <stdlib.h>

int *get_last_elements(const int *const *sublists, const size_t *lengths,
                       size_t list_count)
{
    int *last_elements;
    size_t i;

    if ((sublists == NULL || lengths == NULL) && list_count != 0U) {
        return NULL;
    }

    if (list_count == 0U) {
        return NULL;
    }

    if (list_count > SIZE_MAX / sizeof(*last_elements)) {
        return NULL;
    }

    last_elements = malloc(list_count * sizeof(*last_elements));
    if (last_elements == NULL) {
        return NULL;
    }

    for (i = 0U; i < list_count; ++i) {
        if (sublists[i] == NULL || lengths[i] == 0U) {
            free(last_elements);
            return NULL;
        }

        last_elements[i] = sublists[i][lengths[i] - 1U];
    }

    return last_elements;
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
    const size_t list_count = sizeof(sublists) / sizeof(sublists[0]);
    int *last_elements;
    size_t i;

    last_elements = get_last_elements(sublists, lengths, list_count);
    if (last_elements == NULL) {
        fputs("Failed to get last elements.\n", stderr);
        return EXIT_FAILURE;
    }

    for (i = 0U; i < list_count; ++i) {
        if (printf("%d%s", last_elements[i],
                   i + 1U == list_count ? "\n" : " ") < 0) {
            free(last_elements);
            return EXIT_FAILURE;
        }
    }

    free(last_elements);
    return EXIT_SUCCESS;
}