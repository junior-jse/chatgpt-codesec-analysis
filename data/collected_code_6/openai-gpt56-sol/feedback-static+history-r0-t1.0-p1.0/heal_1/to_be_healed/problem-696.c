#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct {
    int *items;
    size_t length;
} IntList;

typedef struct {
    IntList *items;
    size_t length;
} ListOfLists;

typedef struct {
    IntList first;
    IntList second;
} ListPair;

typedef struct {
    ListPair *items;
    size_t length;
} ZippedLists;

static void free_int_list(IntList *list)
{
    if (list != NULL) {
        free(list->items);
        list->items = NULL;
        list->length = 0;
    }
}

static void free_list_of_lists(ListOfLists *lists)
{
    /* Possible weaknesses found:
     *  The scope of the variable 'i' can be reduced. [variableScope]
     */
    size_t i;

    if (lists != NULL) {
        for (i = 0; i < lists->length; ++i) {
            free_int_list(&lists->items[i]);
        }

        free(lists->items);
        lists->items = NULL;
        lists->length = 0;
    }
}

static void free_zipped_lists(ZippedLists *zipped)
{
    /* Possible weaknesses found:
     *  The scope of the variable 'i' can be reduced. [variableScope]
     */
    size_t i;

    if (zipped != NULL) {
        for (i = 0; i < zipped->length; ++i) {
            free_int_list(&zipped->items[i].first);
            free_int_list(&zipped->items[i].second);
        }

        free(zipped->items);
        zipped->items = NULL;
        zipped->length = 0;
    }
}

static int copy_int_list(const IntList *source, IntList *destination)
{
    if (source == NULL || destination == NULL ||
        (source->length > 0 && source->items == NULL)) {
        return 0;
    }

    destination->items = NULL;
    destination->length = 0;

    if (source->length == 0) {
        return 1;
    }

    if (source->length > SIZE_MAX / sizeof(*destination->items)) {
        return 0;
    }

    destination->items = malloc(source->length * sizeof(*destination->items));
    if (destination->items == NULL) {
        return 0;
    }

    for (size_t i = 0; i < source->length; ++i) {
        destination->items[i] = source->items[i];
    }

    destination->length = source->length;
    return 1;
}

static int zip_lists(const ListOfLists *first,
                     const ListOfLists *second,
                     ZippedLists *result)
{
    size_t count;
    size_t i;

    if (first == NULL || second == NULL || result == NULL ||
        (first->length > 0 && first->items == NULL) ||
        (second->length > 0 && second->items == NULL)) {
        return 0;
    }

    result->items = NULL;
    result->length = 0;
    count = first->length < second->length ? first->length : second->length;

    if (count == 0) {
        return 1;
    }

    if (count > SIZE_MAX / sizeof(*result->items)) {
        return 0;
    }

    result->items = calloc(count, sizeof(*result->items));
    if (result->items == NULL) {
        return 0;
    }

    for (i = 0; i < count; ++i) {
        if (!copy_int_list(&first->items[i], &result->items[i].first) ||
            !copy_int_list(&second->items[i], &result->items[i].second)) {
            result->length = i + 1;
            free_zipped_lists(result);
            return 0;
        }
        result->length = i + 1;
    }

    return 1;
}

static void print_int_list(const IntList *list)
{
    size_t i;

    putchar('[');
    for (i = 0; i < list->length; ++i) {
        if (i > 0) {
            printf(", ");
        }
        printf("%d", list->items[i]);
    }
    putchar(']');
}

static void print_zipped_lists(const ZippedLists *zipped)
{
    size_t i;

    putchar('[');
    for (i = 0; i < zipped->length; ++i) {
        if (i > 0) {
            printf(", ");
        }
        putchar('(');
        print_int_list(&zipped->items[i].first);
        printf(", ");
        print_int_list(&zipped->items[i].second);
        putchar(')');
    }
    puts("]");
}

int main(void)
{
    int first_values_0[] = {1, 2};
    int first_values_1[] = {3, 4, 5};
    int first_values_2[] = {6};
    int second_values_0[] = {10};
    int second_values_1[] = {20, 30};

    IntList first_items[] = {
        {first_values_0, sizeof(first_values_0) / sizeof(first_values_0[0])},
        {first_values_1, sizeof(first_values_1) / sizeof(first_values_1[0])},
        {first_values_2, sizeof(first_values_2) / sizeof(first_values_2[0])}
    };

    IntList second_items[] = {
        {second_values_0, sizeof(second_values_0) / sizeof(second_values_0[0])},
        {second_values_1, sizeof(second_values_1) / sizeof(second_values_1[0])}
    };

    ListOfLists first = {
        first_items,
        sizeof(first_items) / sizeof(first_items[0])
    };

    ListOfLists second = {
        second_items,
        sizeof(second_items) / sizeof(second_items[0])
    };

    ZippedLists zipped = {NULL, 0};

    if (!zip_lists(&first, &second, &zipped)) {
        fputs("Failed to zip lists.\n", stderr);
        free_zipped_lists(&zipped);
        return EXIT_FAILURE;
    }

    print_zipped_lists(&zipped);
    free_zipped_lists(&zipped);
    free_list_of_lists(NULL);

    return EXIT_SUCCESS;
}