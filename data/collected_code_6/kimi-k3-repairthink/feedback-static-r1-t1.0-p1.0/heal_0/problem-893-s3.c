#include <stdio.h>
#include <stdlib.h>

int get_last_elements(int **sublists, const size_t *sizes, size_t count, int *result)
{
    size_t i;

    if (count == 0) {
        return 0;
    }
    if (sublists == NULL || sizes == NULL || result == NULL) {
        return -1;
    }

    for (i = 0; i < count; i++) {
        if (sublists[i] == NULL || sizes[i] == 0) {
            return -1;
        }
        result[i] = sublists[i][sizes[i] - 1];
    }

    return 0;
}

int main(void)
{
    int list1[] = {1, 2, 3};
    int list2[] = {4, 5};
    int list3[] = {6, 7, 8, 9};
    int *sublists[] = {list1, list2, list3};
    size_t sizes[] = {3, 2, 4};
    size_t count = 3;
    int *result = NULL;
    size_t i;

    result = (int *)malloc(count * sizeof(int));
    if (result == NULL) {
        return EXIT_FAILURE;
    }

    if (get_last_elements(sublists, sizes, count, result) != 0) {
        free(result);
        return EXIT_FAILURE;
    }

    for (i = 0; i < count; i++) {
        printf("%d\n", result[i]);
    }

    free(result);
    return EXIT_SUCCESS;
}