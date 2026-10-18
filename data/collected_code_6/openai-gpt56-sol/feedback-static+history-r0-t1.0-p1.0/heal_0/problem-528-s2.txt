#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct {
    int *items;
    size_t length;
} List;

typedef struct {
    const List **items;
    size_t count;
} ListSelection;

static int find_minimum_length_lists(const List *lists, size_t count,
                                     ListSelection *result)
{
    size_t minimum;
    size_t selected_count;
    size_t index;
    const List **selected;

    if (result == NULL || (lists == NULL && count != 0U)) {
        return -1;
    }

    result->items = NULL;
    result->count = 0U;

    if (count == 0U) {
        return 0;
    }

    minimum = lists[0].length;
    for (index = 1U; index < count; ++index) {
        if (lists[index].length < minimum) {
            minimum = lists[index].length;
        }
    }

    selected_count = 0U;
    for (index = 0U; index < count; ++index) {
        if (lists[index].length == minimum) {
            ++selected_count;
        }
    }

    if (selected_count > SIZE_MAX / sizeof(*selected)) {
        return -1;
    }

    selected = malloc(selected_count * sizeof(*selected));
    if (selected == NULL) {
        return -1;
    }

    selected_count = 0U;
    for (index = 0U; index < count; ++index) {
        if (lists[index].length == minimum) {
            selected[selected_count] = &lists[index];
            ++selected_count;
        }
    }

    result->items = selected;
    result->count = selected_count;
    return 0;
}

static void free_list_selection(ListSelection *selection)
{
    if (selection != NULL) {
        free(selection->items);
        selection->items = NULL;
        selection->count = 0U;
    }
}

static void print_list(const List *list)
{
    size_t index;

    printf("[");
    for (index = 0U; index < list->length; ++index) {
        if (index != 0U) {
            printf(", ");
        }
        printf("%d", list->items[index]);
    }
    printf("]\n");
}

int main(void)
{
    int first[] = {1, 2, 3};
    int second[] = {4};
    int third[] = {5, 6};
    int fourth[] = {7};
    List lists[] = {
        {first, sizeof(first) / sizeof(first[0])},
        {second, sizeof(second) / sizeof(second[0])},
        {third, sizeof(third) / sizeof(third[0])},
        {fourth, sizeof(fourth) / sizeof(fourth[0])}
    };
    ListSelection result;
    size_t index;

    if (find_minimum_length_lists(
            lists, sizeof(lists) / sizeof(lists[0]), &result) != 0) {
        fprintf(stderr, "Failed to find minimum-length lists.\n");
        return EXIT_FAILURE;
    }

    for (index = 0U; index < result.count; ++index) {
        print_list(result.items[index]);
    }

    free_list_selection(&result);
    return EXIT_SUCCESS;
}