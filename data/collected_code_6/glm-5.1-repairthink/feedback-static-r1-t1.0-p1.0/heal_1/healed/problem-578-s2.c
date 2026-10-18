#include <stdio.h>
#include <stdlib.h>

int *interleave(const int *list1, const int *list2, size_t length) {
    if (list1 == NULL || list2 == NULL) {
        return NULL;
    }

    if (length == 0) {
        return NULL;
    }

    size_t total_length = length * 2;
    int *result = (int *)malloc(total_length * sizeof(int));
    if (result == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < length; ++i) {
        result[i * 2] = list1[i];
        result[i * 2 + 1] = list2[i];
    }

    return result;
}

int main(void) {
    const int list1[] = {1, 2, 3};
    const int list2[] = {4, 5, 6};
    size_t length = sizeof(list1) / sizeof(list1[0]);

    int *interleaved = interleave(list1, list2, length);
    if (interleaved == NULL) {
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < length * 2; ++i) {
        printf("%d ", interleaved[i]);
    }
    printf("\n");

    free(interleaved);
    interleaved = NULL;

    return EXIT_SUCCESS;
}