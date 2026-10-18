#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int value;
    size_t index;
} Element;

static int compare_elements(const void *left, const void *right)
{
    const Element *a = left;
    const Element *b = right;

    if (a->value < b->value) {
        return -1;
    }
    if (a->value > b->value) {
        return 1;
    }
    return 0;
}

static int find_most_frequent(const int *items, size_t count, int *result)
{
    Element *elements;
    size_t best_count = 0;
    size_t best_index = count;
    size_t i = 0;

    if (items == NULL || result == NULL || count == 0) {
        return 0;
    }

    if (count > SIZE_MAX / sizeof(*elements)) {
        return 0;
    }

    elements = malloc(count * sizeof(*elements));
    if (elements == NULL) {
        return 0;
    }

    for (i = 0; i < count; ++i) {
        elements[i].value = items[i];
        elements[i].index = i;
    }

    qsort(elements, count, sizeof(*elements), compare_elements);

    i = 0;
    while (i < count) {
        size_t j = i + 1;
        size_t first_index = elements[i].index;

        while (j < count && elements[j].value == elements[i].value) {
            if (elements[j].index < first_index) {
                first_index = elements[j].index;
            }
            ++j;
        }

        if (j - i > best_count ||
            (j - i == best_count && first_index < best_index)) {
            best_count = j - i;
            best_index = first_index;
            *result = elements[i].value;
        }

        i = j;
    }

    free(elements);
    return 1;
}

int main(void)
{
    int items[] = {4, 2, 7, 2, 4, 2, 7, 7, 7};
    const size_t count = sizeof(items) / sizeof(items[0]);
    int result = 0;

    if (!find_most_frequent(items, count, &result)) {
        fputs("Unable to find the most frequent item.\n", stderr);
        return EXIT_FAILURE;
    }

    if (printf("%d\n", result) < 0) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}