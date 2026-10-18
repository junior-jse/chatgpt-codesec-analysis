#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

static void swap_int(int *a, int *b)
{
    int temporary = *a;
    *a = *b;
    *b = temporary;
}

static void sift_down(int *values, size_t count, size_t root)
{
    while (root <= (count - 2U) / 2U) {
        size_t child = root * 2U + 1U;

        if (child + 1U < count && values[child] < values[child + 1U]) {
            ++child;
        }

        if (values[root] >= values[child]) {
            return;
        }

        swap_int(&values[root], &values[child]);
        root = child;
    }
}

static int make_heap(int *values, size_t count)
{
    if (count > 0U && values == NULL) {
        return -1;
    }

    if (count < 2U) {
        return 0;
    }

    for (size_t parent = (count - 2U) / 2U + 1U; parent > 0U; --parent) {
        sift_down(values, count, parent - 1U);
    }

    return 0;
}

int main(void)
{
    size_t count;

    if (scanf("%zu", &count) != 1) {
        fputs("Failed to read the element count.\n", stderr);
        return EXIT_FAILURE;
    }

    if (count > SIZE_MAX / sizeof(int)) {
        fputs("The requested list is too large.\n", stderr);
        return EXIT_FAILURE;
    }

    int *values = NULL;

    if (count > 0U) {
        values = malloc(count * sizeof(*values));
        if (values == NULL) {
            fputs("Memory allocation failed.\n", stderr);
            return EXIT_FAILURE;
        }
    }

    for (size_t i = 0U; i < count; ++i) {
        if (scanf("%d", &values[i]) != 1) {
            fputs("Failed to read a list element.\n", stderr);
            free(values);
            return EXIT_FAILURE;
        }
    }

    if (make_heap(values, count) != 0) {
        fputs("Failed to construct the heap.\n", stderr);
        free(values);
        return EXIT_FAILURE;
    }

    for (size_t i = 0U; i < count; ++i) {
        if (printf("%d%s", values[i], i + 1U == count ? "\n" : " ") < 0) {
            free(values);
            return EXIT_FAILURE;
        }
    }

    free(values);
    return EXIT_SUCCESS;
}