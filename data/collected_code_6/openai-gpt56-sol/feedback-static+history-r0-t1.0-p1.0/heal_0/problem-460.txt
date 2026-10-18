#include <stdio.h>
#include <stdlib.h>

static int get_first_elements(const int *const *sublists,
                              const size_t *lengths,
                              size_t sublist_count,
                              int **result,
                              size_t *result_count)
{
    int *values;
    size_t i;

    if (sublists == NULL || lengths == NULL || result == NULL ||
        result_count == NULL || sublist_count == 0 ||
        sublist_count > SIZE_MAX / sizeof(*values)) {
        return -1;
    }

    values = malloc(sublist_count * sizeof(*values));
    if (values == NULL) {
        return -1;
    }

    for (i = 0; i < sublist_count; ++i) {
        if (sublists[i] == NULL || lengths[i] == 0) {
            free(values);
            return -1;
        }
        values[i] = sublists[i][0];
    }

    *result = values;
    *result_count = sublist_count;
    return 0;
}

int main(void)
{
    const int list1[] = {1, 2, 3};
    const int list2[] = {4, 5};
    const int list3[] = {6, 7, 8, 9};
    const int *sublists[] = {list1, list2, list3};
    const size_t lengths[] = {
        sizeof(list1) / sizeof(list1[0]),
        sizeof(list2) / sizeof(list2[0]),
        sizeof(list3) / sizeof(list3[0])
    };
    int *first_elements = NULL;
    size_t count = 0;
    size_t i;

    if (get_first_elements(sublists,
                           lengths,
                           sizeof(sublists) / sizeof(sublists[0]),
                           &first_elements,
                           &count) != 0) {
        fputs("Failed to get first elements.\n", stderr);
        return EXIT_FAILURE;
    }

    for (i = 0; i < count; ++i) {
        if (printf("%d%s", first_elements[i], i + 1 == count ? "\n" : " ") < 0) {
            free(first_elements);
            return EXIT_FAILURE;
        }
    }

    free(first_elements);
    return EXIT_SUCCESS;
}