#include <stdio.h>
#include <stdlib.h>

int *get_last_elements(int **sublists, const size_t *sizes, size_t count, size_t *out_count)
{
    int *result;
    size_t i;
    size_t valid_count;

    if (out_count == NULL) {
        return NULL;
    }
    *out_count = 0;

    if (sublists == NULL || sizes == NULL || count == 0) {
        return NULL;
    }

    result = (int *)malloc(count * sizeof(int));
    if (result == NULL) {
        return NULL;
    }

    valid_count = 0;
    for (i = 0; i < count; i++) {
        if (sublists[i] != NULL && sizes[i] > 0) {
            result[valid_count] = sublists[i][sizes[i] - 1];
            valid_count++;
        }
    }

    if (valid_count == 0) {
        free(result);
        return NULL;
    }

    *out_count = valid_count;
    return result;
}

int main(void)
{
    int list1[] = {1, 2, 3};
    int list2[] = {4, 5};
    int list3[] = {6, 7, 8, 9};
    int *sublists[] = {list1, list2, list3};
    size_t sizes[] = {3, 2, 4};
    size_t count = 3;
    size_t out_count = 0;
    int *last_elements;
    size_t i;

    last_elements = get_last_elements(sublists, sizes, count, &out_count);
    if (last_elements == NULL) {
        return EXIT_FAILURE;
    }

    for (i = 0; i < out_count; i++) {
        printf("%d\n", last_elements[i]);
    }

    free(last_elements);
    return EXIT_SUCCESS;
}