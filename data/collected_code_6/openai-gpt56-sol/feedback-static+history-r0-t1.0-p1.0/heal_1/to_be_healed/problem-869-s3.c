#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

typedef struct {
    int *values;
    size_t length;
} Sublist;

typedef struct {
    Sublist *sublists;
    size_t length;
} ListOfLists;

static void free_sublist(Sublist *sublist)
{
    if (sublist != NULL) {
        free(sublist->values);
        sublist->values = NULL;
        sublist->length = 0;
    }
}

static void free_list_of_lists(ListOfLists *list)
{
    /* Possible weaknesses found:
     *  The scope of the variable 'i' can be reduced. [variableScope]
     */
    size_t i;

    if (list != NULL) {
        for (i = 0; i < list->length; ++i) {
            free_sublist(&list->sublists[i]);
        }

        free(list->sublists);
        list->sublists = NULL;
        list->length = 0;
    }
}

static int sublist_is_in_range(const Sublist *sublist, int minimum, int maximum)
{
    size_t i;

    if (sublist == NULL || (sublist->length > 0 && sublist->values == NULL)) {
        return 0;
    }

    for (i = 0; i < sublist->length; ++i) {
        if (sublist->values[i] < minimum || sublist->values[i] > maximum) {
            return 0;
        }
    }

    return 1;
}

static int remove_sublists_outside_range(
    ListOfLists *list,
    int minimum,
    int maximum)
{
    size_t read_index;
    size_t write_index = 0;

    if (list == NULL || minimum > maximum ||
        (list->length > 0 && list->sublists == NULL)) {
        return -1;
    }

    for (read_index = 0; read_index < list->length; ++read_index) {
        if (sublist_is_in_range(
                &list->sublists[read_index], minimum, maximum)) {
            if (write_index != read_index) {
                list->sublists[write_index] = list->sublists[read_index];
                list->sublists[read_index].values = NULL;
                list->sublists[read_index].length = 0;
            }
            ++write_index;
        } else {
            free_sublist(&list->sublists[read_index]);
        }
    }

    list->length = write_index;

    if (write_index == 0) {
        free(list->sublists);
        list->sublists = NULL;
    } else {
        Sublist *resized = realloc(
            list->sublists, write_index * sizeof(*list->sublists));

        if (resized != NULL) {
            list->sublists = resized;
        }
    }

    return 0;
}

static int initialize_sublist(
    Sublist *sublist,
    const int *values,
    size_t length)
{
    if (sublist == NULL || (length > 0 && values == NULL)) {
        return -1;
    }

    sublist->values = NULL;
    sublist->length = 0;

    if (length == 0) {
        return 0;
    }

    if (length > SIZE_MAX / sizeof(*sublist->values)) {
        return -1;
    }

    sublist->values = malloc(length * sizeof(*sublist->values));
    if (sublist->values == NULL) {
        return -1;
    }

    for (size_t i = 0; i < length; ++i) {
        sublist->values[i] = values[i];
    }

    sublist->length = length;
    return 0;
}

static void print_list_of_lists(const ListOfLists *list)
{
    size_t i;
    size_t j;

    if (list == NULL) {
        return;
    }

    for (i = 0; i < list->length; ++i) {
        putchar('[');
        for (j = 0; j < list->sublists[i].length; ++j) {
            if (j > 0) {
                printf(", ");
            }
            printf("%d", list->sublists[i].values[j]);
        }
        puts("]");
    }
}

int main(void)
{
    const int data[][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {-1, 2, 3},
        {7, 8, 9}
    };
    const size_t sublist_count = sizeof(data) / sizeof(data[0]);
    ListOfLists list = {NULL, 0};
    size_t i;

    if (sublist_count > SIZE_MAX / sizeof(*list.sublists)) {
        return EXIT_FAILURE;
    }

    list.sublists = calloc(sublist_count, sizeof(*list.sublists));
    if (list.sublists == NULL) {
        return EXIT_FAILURE;
    }
    list.length = sublist_count;

    for (i = 0; i < sublist_count; ++i) {
        if (initialize_sublist(
                &list.sublists[i],
                data[i],
                sizeof(data[i]) / sizeof(data[i][0])) != 0) {
            free_list_of_lists(&list);
            return EXIT_FAILURE;
        }
    }

    if (remove_sublists_outside_range(&list, 1, 8) != 0) {
        free_list_of_lists(&list);
        return EXIT_FAILURE;
    }

    print_list_of_lists(&list);
    free_list_of_lists(&list);
    return EXIT_SUCCESS;
}