#include <stdio.h>
#include <stdlib.h>

static int *remove_even_numbers(const int *list, size_t length, size_t *result_length)
{
    int *result;
    size_t count = 0;
    size_t index = 0;

    if (result_length == NULL || (list == NULL && length != 0)) {
        return NULL;
    }

    *result_length = 0;

    for (size_t i = 0; i < length; ++i) {
        if (list[i] % 2 != 0) {
            ++count;
        }
    }

    if (count == 0) {
        return NULL;
    }

    if (count > SIZE_MAX / sizeof(*result)) {
        return NULL;
    }

    result = malloc(count * sizeof(*result));
    if (result == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < length; ++i) {
        if (list[i] % 2 != 0) {
            result[index++] = list[i];
        }
    }

    *result_length = count;
    return result;
}

int main(void)
{
    int list[] = {1, 2, 3, 4, 5, 6, 7, 8};
    const size_t length = sizeof(list) / sizeof(list[0]);
    size_t result_length = 0;
    int *result = remove_even_numbers(list, length, &result_length);

    if (result == NULL && result_length != 0) {
        fputs("Failed to remove even numbers.\n", stderr);
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < result_length; ++i) {
        if (printf("%d%s", result[i], i + 1 < result_length ? " " : "\n") < 0) {
            free(result);
            return EXIT_FAILURE;
        }
    }

    if (result_length == 0 && putchar('\n') == EOF) {
        free(result);
        return EXIT_FAILURE;
    }

    free(result);
    return EXIT_SUCCESS;
}