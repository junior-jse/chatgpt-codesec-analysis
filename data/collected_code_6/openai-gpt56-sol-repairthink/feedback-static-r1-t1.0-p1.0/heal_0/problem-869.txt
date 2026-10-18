#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *items;
    size_t count;
} Sublist;

typedef struct {
    Sublist *items;
    size_t count;
} List;

static void free_sublist(Sublist *sublist)
{
    if (sublist == NULL) {
        return;
    }

    free(sublist->items);
    sublist->items = NULL;
    sublist->count = 0;
}

static void free_list(List *list)
{
    size_t i;

    if (list == NULL) {
        return;
    }

    for (i = 0; i < list->count; ++i) {
        free_sublist(&list->items[i]);
    }

    free(list->items);
    list->items = NULL;
    list->count = 0;
}

static int sublist_is_in_range(const Sublist *sublist, int minimum, int maximum)
{
    size_t i;

    if (sublist == NULL) {
        return 0;
    }

    for (i = 0; i < sublist->count; ++i) {
        if (sublist->items[i] < minimum || sublist->items[i] > maximum) {
            return 0;
        }
    }

    return 1;
}

static void remove_sublists_outside_range(List *list, int minimum, int maximum)
{
    size_t read_index;
    size_t write_index = 0;

    if (list == NULL || minimum > maximum) {
        return;
    }

    for (read_index = 0; read_index < list->count; ++read_index) {
        if (sublist_is_in_range(&list->items[read_index], minimum, maximum)) {
            if (write_index != read_index) {
                list->items[write_index] = list->items[read_index];
                list->items[read_index].items = NULL;
                list->items[read_index].count = 0;
            }
            ++write_index;
        } else {
            free_sublist(&list->items[read_index]);
        }
    }

    list->count = write_index;

    if (write_index == 0) {
        free(list->items);
        list->items = NULL;
    } else {
        Sublist *resized = realloc(list->items, write_index * sizeof(*resized));

        if (resized != NULL) {
            list->items = resized;
        }
    }
}

static int initialize_sublist(Sublist *sublist, const int *values, size_t count)
{
    if (sublist == NULL || (count > 0 && values == NULL)) {
        return 0;
    }

    sublist->items = NULL;
    sublist->count = 0;

    if (count == 0) {
        return 1;
    }

    if (count > SIZE_MAX / sizeof(*sublist->items)) {
        return 0;
    }

    sublist->items = malloc(count * sizeof(*sublist->items));
    if (sublist->items == NULL) {
        return 0;
    }

    for (size_t i = 0; i < count; ++i) {
        sublist->items[i] = values[i];
    }

    sublist->count = count;
    return 1;
}

static void print_list(const List *list)
{
    size_t i;
    size_t j;

    if (list == NULL) {
        return;
    }

    for (i = 0; i < list->count; ++i) {
        putchar('[');
        for (j = 0; j < list->items[i].count; ++j) {
            if (j != 0) {
                printf(", ");
            }
            printf("%d", list->items[i].items[j]);
        }
        puts("]");
    }
}

int main(void)
{
    const int values[][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {-1, 2, 3},
        {7, 8, 9}
    };
    const size_t sublist_count = sizeof(values) / sizeof(values[0]);
    List list = {NULL, 0};
    size_t i;

    if (sublist_count > SIZE_MAX / sizeof(*list.items)) {
        return EXIT_FAILURE;
    }

    list.items = calloc(sublist_count, sizeof(*list.items));
    if (list.items == NULL) {
        return EXIT_FAILURE;
    }
    list.count = sublist_count;

    for (i = 0; i < sublist_count; ++i) {
        if (!initialize_sublist(
                &list.items[i],
                values[i],
                sizeof(values[i]) / sizeof(values[i][0]))) {
            free_list(&list);
            return EXIT_FAILURE;
        }
    }

    remove_sublists_outside_range(&list, 1, 8);
    print_list(&list);
    free_list(&list);

    return EXIT_SUCCESS;
}