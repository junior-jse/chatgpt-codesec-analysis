#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int extract_elements(const int *source, size_t source_length,
                     size_t start, size_t count, int **result)
{
    int *extracted;
    size_t i;

    if (result == NULL) {
        return -1;
    }

    *result = NULL;

    if (source == NULL || start > source_length ||
        count > source_length - start) {
        return -1;
    }

    if (count == 0) {
        return 0;
    }

    if (count > SIZE_MAX / sizeof(*extracted)) {
        return -1;
    }

    extracted = malloc(count * sizeof(*extracted));
    if (extracted == NULL) {
        return -1;
    }

    for (i = 0; i < count; ++i) {
        extracted[i] = source[start + i];
    }

    *result = extracted;
    return 0;
}

int main(void)
{
    const int list[] = {10, 20, 30, 40, 50, 60};
    const size_t list_length = sizeof(list) / sizeof(list[0]);
    const size_t start = 2;
    const size_t count = 3;
    int *extracted = NULL;
    size_t i;

    if (extract_elements(list, list_length, start, count, &extracted) != 0) {
        fputs("Failed to extract elements.\n", stderr);
        return EXIT_FAILURE;
    }

    for (i = 0; i < count; ++i) {
        printf("%d%s", extracted[i], i + 1 == count ? "\n" : " ");
    }

    free(extracted);
    return EXIT_SUCCESS;
}