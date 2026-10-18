#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *items;
    size_t length;
} IntList;

static int add_lists(const IntList *a, const IntList *b, IntList *result)
{
    size_t i;

    if (a == NULL || b == NULL || result == NULL) {
        return EINVAL;
    }

    result->items = NULL;
    result->length = 0;

    if (a->length != b->length) {
        return EINVAL;
    }

    if (a->length > 0 && (a->items == NULL || b->items == NULL)) {
        return EINVAL;
    }

    if (a->length > SIZE_MAX / sizeof(*result->items)) {
        return EOVERFLOW;
    }

    if (a->length == 0) {
        return 0;
    }

    result->items = malloc(a->length * sizeof(*result->items));
    if (result->items == NULL) {
        return ENOMEM;
    }

    for (i = 0; i < a->length; ++i) {
        if ((b->items[i] > 0 && a->items[i] > INT32_MAX - b->items[i]) ||
            (b->items[i] < 0 && a->items[i] < INT32_MIN - b->items[i])) {
            free(result->items);
            result->items = NULL;
            return EOVERFLOW;
        }

        result->items[i] = a->items[i] + b->items[i];
    }

    result->length = a->length;
    return 0;
}

static void free_list(IntList *list)
{
    if (list != NULL) {
        free(list->items);
        list->items = NULL;
        list->length = 0;
    }
}

int main(void)
{
    int first_items[] = {1, 2, 3, 4};
    int second_items[] = {5, 6, 7, 8};
    IntList first = {first_items, sizeof(first_items) / sizeof(first_items[0])};
    IntList second = {second_items, sizeof(second_items) / sizeof(second_items[0])};
    IntList result = {NULL, 0};
    int status;
    size_t i;

    status = add_lists(&first, &second, &result);
    if (status != 0) {
        fprintf(stderr, "Failed to add lists: error %d\n", status);
        return EXIT_FAILURE;
    }

    for (i = 0; i < result.length; ++i) {
        if (printf("%d%s", result.items[i],
                   i + 1 == result.length ? "\n" : " ") < 0) {
            free_list(&result);
            return EXIT_FAILURE;
        }
    }

    free_list(&result);
    return EXIT_SUCCESS;
}