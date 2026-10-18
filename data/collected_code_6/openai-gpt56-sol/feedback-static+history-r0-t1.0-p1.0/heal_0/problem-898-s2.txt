#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int *extract_elements(const int *list, size_t list_size, size_t start,
                      size_t count)
{
    int *result;

    if (list == NULL || count == 0 || start > list_size ||
        count > list_size - start || count > SIZE_MAX / sizeof(*result)) {
        return NULL;
    }

    result = malloc(count * sizeof(*result));
    if (result == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < count; ++i) {
        result[i] = list[start + i];
    }

    return result;
}

int main(void)
{
    const int list[] = {10, 20, 30, 40, 50, 60};
    const size_t list_size = sizeof(list) / sizeof(list[0]);
    const size_t start = 2;
    const size_t count = 3;
    int *extracted = extract_elements(list, list_size, start, count);

    if (extracted == NULL) {
        fputs("Unable to extract elements.\n", stderr);
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < count; ++i) {
        printf("%d%s", extracted[i], i + 1 == count ? "\n" : " ");
    }

    free(extracted);
    return EXIT_SUCCESS;
}