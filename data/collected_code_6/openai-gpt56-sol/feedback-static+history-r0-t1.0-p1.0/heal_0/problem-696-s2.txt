#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

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

    if (value->lists != NULL) {
        for (size_t i = 0; i < value->length; ++i) {
            free(value->lists[i].items);
            value->lists[i].items = NULL;
            value->lists[i].length = 0;
        }
        free(value->lists);
    }

    value->lists = NULL;
    value->length = 0;
}

static void free_zipped_lists(ZippedLists *value)
{
    if (value == NULL) {
        return;
    }

    if (value->lists != NULL) {
        for (size_t i = 0; i < value->length; ++i) {
            free(value->lists[i].items);
            value->lists[i].items = NULL;
            value->lists[i].length = 0;
        }
        free(value->lists);
    }

    value->lists = NULL;
    value->length = 0;
}

static int create_list_of_lists(
    ListOfLists *result,
    const int *const arrays[],
    const size_t lengths[],
    size_t count)
{
    if (result == NULL || (count != 0 && (arrays == NULL || lengths == NULL))) {
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

        if (arrays[i] == NULL || lengths[i] > SIZE_MAX / sizeof(int)) {
            free_list_of_lists(result);
            return 0;
        }

        result->lists[i].items = malloc(lengths[i] * sizeof(int));
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

static int zip_lists_of_lists(
    const ListOfLists *first,
    const ListOfLists *second,
    ZippedLists *result)
{
    if (first == NULL || second == NULL || result == NULL) {
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
        size_t inner_length = first->lists[i].length < second->lists[i].length
            ? first->lists[i].length
            : second->lists[i].length;

        if (inner_length == 0) {
            continue;
        }

        if (first->lists[i].items == NULL || second->lists[i].items == NULL ||
            inner_length > SIZE_MAX / sizeof(IntPair)) {
            free_zipped_lists(result);
            return 0;
        }

        result->lists[i].items = malloc(inner_length * sizeof(IntPair));
        if (result->lists[i].items == NULL) {
            free_zipped_lists(result);
            return 0;
        }

        result->lists[i].length = inner_length;
        for (size_t j = 0; j < inner_length; ++j) {
            result->lists[i].items[j].first = first->lists[i].items[j];
            result->lists[i].items[j].second = second->lists[i].items[j];
        }
    }

    return 1;
}

static int print_zipped_lists(const ZippedLists *value)
{
    if (value == NULL) {
        return 0;
    }

    if (printf("[") < 0) {
        return 0;
    }

    for (size_t i = 0; i < value->length; ++i) {
        if (i != 0 && printf(", ") < 0) {
            return 0;
        }
        if (printf("[") < 0) {
            return 0;
        }

        for (size_t j = 0; j < value->lists[i].length; ++j) {
            if (j != 0 && printf(", ") < 0) {
                return 0;
            }
            if (printf(
                    "(%d, %d)",
                    value->lists[i].items[j].first,
                    value->lists[i].items[j].second) < 0) {
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
    const int second_0[] = {10, 20};
    const int second_1[] = {30, 40, 50};

    const int *first_arrays[] = {first_0, first_1};
    const int *second_arrays[] = {second_0, second_1};
    const size_t first_lengths[] = {3, 2};
    const size_t second_lengths[] = {2, 3};

    ListOfLists first = {NULL, 0};
    ListOfLists second = {NULL, 0};
    ZippedLists zipped = {NULL, 0};
    int status = EXIT_FAILURE;

    if (!create_list_of_lists(&first, first_arrays, first_lengths, 2)) {
        goto cleanup;
    }

    if (!create_list_of_lists(&second, second_arrays, second_lengths, 2)) {
        goto cleanup;
    }

    if (!zip_lists_of_lists(&first, &second, &zipped)) {
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