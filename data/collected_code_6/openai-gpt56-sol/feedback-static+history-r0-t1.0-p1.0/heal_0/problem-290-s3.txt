#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *items;
    size_t length;
} List;

typedef struct {
    List **items;
    size_t count;
} ListSelection;

static int find_longest_lists(List *lists, size_t list_count, ListSelection *result)
{
    size_t max_length = 0;
    size_t match_count = 0;
    size_t i;

    if (result == NULL || (lists == NULL && list_count != 0)) {
        return -1;
    }

    result->items = NULL;
    result->count = 0;

    if (list_count == 0) {
        return 0;
    }

    for (i = 0; i < list_count; ++i) {
        if (lists[i].length > max_length) {
            max_length = lists[i].length;
            match_count = 1;
        } else if (lists[i].length == max_length) {
            ++match_count;
        }
    }

    if (match_count > SIZE_MAX / sizeof(*result->items)) {
        return -1;
    }

    result->items = malloc(match_count * sizeof(*result->items));
    if (result->items == NULL) {
        return -1;
    }

    for (i = 0; i < list_count; ++i) {
        if (lists[i].length == max_length) {
            result->items[result->count++] = &lists[i];
        }
    }

    return 0;
}

static void free_selection(ListSelection *selection)
{
    if (selection != NULL) {
        free(selection->items);
        selection->items = NULL;
        selection->count = 0;
    }
}

static void print_list(const List *list)
{
    size_t i;

    if (list == NULL || (list->items == NULL && list->length != 0)) {
        return;
    }

    putchar('[');
    for (i = 0; i < list->length; ++i) {
        if (i != 0) {
            fputs(", ", stdout);
        }
        printf("%d", list->items[i]);
    }
    putchar(']');
}

int main(void)
{
    int values1[] = {1, 2, 3};
    int values2[] = {4, 5, 6, 7, 8};
    int values3[] = {9, 10};
    int values4[] = {11, 12, 13, 14, 15};
    List lists[] = {
        {values1, sizeof(values1) / sizeof(values1[0])},
        {values2, sizeof(values2) / sizeof(values2[0])},
        {values3, sizeof(values3) / sizeof(values3[0])},
        {values4, sizeof(values4) / sizeof(values4[0])}
    };
    ListSelection longest = {NULL, 0};
    size_t i;

    if (find_longest_lists(lists, sizeof(lists) / sizeof(lists[0]), &longest) != 0) {
        fputs("Failed to find longest lists.\n", stderr);
        return EXIT_FAILURE;
    }

    for (i = 0; i < longest.count; ++i) {
        print_list(longest.items[i]);
        putchar('\n');
    }

    free_selection(&longest);

    if (ferror(stdout)) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}