#include <stdio.h>
#include <stdlib.h>

static void swap_ints(int *a, int *b)
{
    int temporary = *a;
    *a = *b;
    *b = temporary;
}

static void sift_down(int *values, size_t count, size_t root)
{
    for (;;) {
        size_t child;
        size_t candidate = root;

        if (root > (count - 2U) / 2U) {
            return;
        }

        child = root * 2U + 1U;

        if (values[child] > values[candidate]) {
            candidate = child;
        }

        if (child + 1U < count && values[child + 1U] > values[candidate]) {
            candidate = child + 1U;
        }

        if (candidate == root) {
            return;
        }

        swap_ints(&values[root], &values[candidate]);
        root = candidate;
    }
}

static int build_max_heap(int *values, size_t count)
{
    size_t index;

    if (values == NULL && count != 0U) {
        return -1;
    }

    if (count < 2U) {
        return 0;
    }

    index = count / 2U;
    while (index > 0U) {
        --index;
        sift_down(values, count, index);
    }

    return 0;
}

int main(void)
{
    int *values = NULL;
    size_t count;
    size_t index;

    printf("Enter the number of elements: ");
    if (scanf("%zu", &count) != 1) {
        fprintf(stderr, "Invalid element count.\n");
        return EXIT_FAILURE;
    }

    if (count > SIZE_MAX / sizeof(*values)) {
        fprintf(stderr, "Element count is too large.\n");
        return EXIT_FAILURE;
    }

    if (count != 0U) {
        values = malloc(count * sizeof(*values));
        if (values == NULL) {
            fprintf(stderr, "Memory allocation failed.\n");
            return EXIT_FAILURE;
        }
    }

    printf("Enter %zu integers: ", count);
    for (index = 0U; index < count; ++index) {
        if (scanf("%d", &values[index]) != 1) {
            fprintf(stderr, "Invalid integer input.\n");
            free(values);
            return EXIT_FAILURE;
        }
    }

    if (build_max_heap(values, count) != 0) {
        fprintf(stderr, "Unable to build heap.\n");
        free(values);
        return EXIT_FAILURE;
    }

    printf("Max heap:");
    for (index = 0U; index < count; ++index) {
        printf(" %d", values[index]);
    }
    putchar('\n');

    free(values);
    return EXIT_SUCCESS;
}