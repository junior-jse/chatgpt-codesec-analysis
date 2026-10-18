#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct {
    int *items;
    size_t length;
} IntList;

typedef struct {
    IntList *lists;
    size_t length;
} ListOfLists;

typedef struct {
    int first;
    int second;
} IntPair;

typedef struct {
    IntPair *items;
    size_t length;
} PairList;

typedef struct {
    PairList *lists;
    size_t length;
} ZippedLists;

static void free_list_of_lists(ListOfLists *value)
{
    if (value == NULL) {
        return;
    }

    for (size_t i = 0; i < value->length; ++i) {
        free(value->lists[i].items);
        value->lists[i].items = NULL;
        value->lists[i].length = 0;
    }

    free(value->lists);
    value->lists = NULL;
    value->length = 0;
}

static void free_zipped_lists(ZippedLists *value)
{
    if (value == NULL) {
        return;
    }

    for (size_t i = 0; i < value->length; ++i) {
        free(value->lists[i].items);
        value->lists[i].items = NULL;
        value->lists[i].length = 0;
    }

    free(value->lists);
    value->lists = NULL;
    value->length = 0;
}

static int make_list_of_lists(
    ListOfLists *result,
    const int *const arrays[],
    const size_t lengths[],
    size_t count)
{
    if (result == NULL || (count > 0 && (arrays == NULL || lengths == NULL))) {
        return 0;
    }

    result->lists = NULL;
    result->length = 0;

    if (count == 0) {
        return 1;
    }

    if (count > SIZE_MAX / sizeof(*result->lists)) {
        return 0;
    }

    result->lists = calloc(count, sizeof(*result->lists));
    if (result->lists == NULL) {
        return 0;
    }

    result->length = count;

    for (size_t i = 0; i < count; ++i) {
        if (lengths[i] == 0) {
            continue;
        }

        if (arrays[i] == NULL ||
            lengths[i] > SIZE_MAX / sizeof(*result->lists[i].items)) {
            free_list_of_lists(result);
            return 0;
        }

        result->lists[i].items =
            malloc(lengths[i] * sizeof(*result->lists[i].items));

        if (result->lists[i].items == NULL) {
            free_list_of_lists(result);
            return 0;
        }

        result->lists[i].length = lengths[i];

        for (size_t j = 0; j < lengths[i]; ++j) {
            result->lists[i].items[j] = arrays[i][j];
        }
    }

    return 1;
}

static int zip_lists(
    const ListOfLists *first,
    const ListOfLists *second,
    ZippedLists *result)
{
    if (first == NULL || second == NULL || result == NULL ||
        (first->length > 0 && first->lists == NULL) ||
        (second->length > 0 && second->lists == NULL)) {
        return 0;
    }

    result->lists = NULL;
    result->length = 0;

    size_t outer_length =
        first->length < second->length ? first->length : second->length;

    if (outer_length == 0) {
        return 1;
    }

    if (outer_length > SIZE_MAX / sizeof(*result->lists)) {
        return 0;
    }

    result->lists = calloc(outer_length, sizeof(*result->lists));
    if (result->lists == NULL) {
        return 0;
    }

    result->length = outer_length;

    for (size_t i = 0; i < outer_length; ++i) {
        const IntList *left = &first->lists[i];
        const IntList *right = &second->lists[i];

        if ((left->length > 0 && left->items == NULL) ||
            (right->length > 0 && right->items == NULL)) {
            free_zipped_lists(result);
            return 0;
        }

        size_t inner_length =
            left->length < right->length ? left->length : right->length;

        if (inner_length == 0) {
            continue;
        }

        if (inner_length > SIZE_MAX / sizeof(*result->lists[i].items)) {
            free_zipped_lists(result);
            return 0;
        }

        result->lists[i].items =
            malloc(inner_length * sizeof(*result->lists[i].items));

        if (result->lists[i].items == NULL) {
            free_zipped_lists(result);
            return 0;
        }

        result->lists[i].length = inner_length;

        for (size_t j = 0; j < inner_length; ++j) {
            result->lists[i].items[j].first = left->items[j];
            result->lists[i].items[j].second = right->items[j];
        }
    }

    return 1;
}

static int print_zipped_lists(const ZippedLists *value)
{
    if (value == NULL || (value->length > 0 && value->lists == NULL)) {
        return 0;
    }

    if (printf("[") < 0) {
        return 0;
    }

    for (size_t i = 0; i < value->length; ++i) {
        const PairList *list = &value->lists[i];

        if (list->length > 0 && list->items == NULL) {
            return 0;
        }

        if (i > 0 && printf(", ") < 0) {
            return 0;
        }

        if (printf("[") < 0) {
            return 0;
        }

        for (size_t j = 0; j < list->length; ++j) {
            if (j > 0 && printf(", ") < 0) {
                return 0;
            }

            if (printf("(%d, %d)",
                       list->items[j].first,
                       list->items[j].second) < 0) {
                return 0;
            }
        }

        if (printf("]") < 0) {
            return 0;
        }
    }

    return printf("]\n") >= 0;
}

int main(void)
{
    const int first_0[] = {1, 2, 3};
    const int first_1[] = {4, 5};
    const int first_2[] = {6, 7, 8, 9};
    const int second_0[] = {10, 20};
    const int second_1[] = {30, 40, 50};
    const int second_2[] = {60, 70, 80, 90};

    const int *first_arrays[] = {first_0, first_1, first_2};
    const int *second_arrays[] = {second_0, second_1, second_2};
    const size_t first_lengths[] = {3, 2, 4};
    const size_t second_lengths[] = {2, 3, 4};

    ListOfLists first = {NULL, 0};
    ListOfLists second = {NULL, 0};
    ZippedLists zipped = {NULL, 0};
    int status = EXIT_FAILURE;

    if (!make_list_of_lists(&first, first_arrays, first_lengths, 3)) {
        goto cleanup;
    }

    if (!make_list_of_lists(&second, second_arrays, second_lengths, 3)) {
        goto cleanup;
    }

    if (!zip_lists(&first, &second, &zipped)) {
        goto cleanup;
    }

    if (!print_zipped_lists(&zipped)) {
        goto cleanup;
    }

    status = EXIT_SUCCESS;

cleanup:
    free_zipped_lists(&zipped);
    free_list_of_lists(&second);
    free_list_of_lists(&first);
    return status;
}