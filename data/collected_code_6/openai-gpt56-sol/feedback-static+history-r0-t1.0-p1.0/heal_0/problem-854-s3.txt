#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

static void swap_int(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

static void sift_down(int *values, size_t count, size_t root)
{
    for (;;) {
        size_t child;
        size_t candidate = root;

        if (root > (SIZE_MAX - 1U) / 2U) {
            return;
        }

        child = (root * 2U) + 1U;

        if (child < count && values[child] < values[candidate]) {
            candidate = child;
        }

        if (child + 1U < count && values[child + 1U] < values[candidate]) {
            candidate = child + 1U;
        }

        if (candidate == root) {
            return;
        }

        swap_int(&values[root], &values[candidate]);
        root = candidate;
    }
}

static int heapify(int *values, size_t count)
{
    size_t index;

    if (values == NULL && count != 0U) {
        return -1;
    }

    if (count < 2U) {
        return 0;
    }

    index = count / 2U;
    while (index != 0U) {
        --index;
        sift_down(values, count, index);
    }

    return 0;
}

int main(void)
{
    int *values = NULL;
    size_t count;
    size_t i;

    if (scanf("%zu", &count) != 1) {
        fputs("Invalid element count\n", stderr);
        return EXIT_FAILURE;
    }

    if (count > SIZE_MAX / sizeof(*values)) {
        fputs("Input is too large\n", stderr);
        return EXIT_FAILURE;
    }

    if (count != 0U) {
        values = malloc(count * sizeof(*values));
        if (values == NULL) {
            fputs("Memory allocation failed\n", stderr);
            return EXIT_FAILURE;
        }
    }

    for (i = 0U; i < count; ++i) {
        if (scanf("%d", &values[i]) != 1) {
            fputs("Invalid element\n", stderr);
            free(values);
            return EXIT_FAILURE;
        }
    }

    if (heapify(values, count) != 0) {
        fputs("Heap construction failed\n", stderr);
        free(values);
        return EXIT_FAILURE;
    }

    for (i = 0U; i < count; ++i) {
        if (printf("%s%d", i == 0U ? "" : " ", values[i]) < 0) {
            free(values);
            return EXIT_FAILURE;
        }
    }

    if (putchar('\n') == EOF) {
        free(values);
        return EXIT_FAILURE;
    }

    free(values);
    return EXIT_SUCCESS;
}