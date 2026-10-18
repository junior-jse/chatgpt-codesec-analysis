#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

typedef struct {
    int *items;
    size_t length;
} IntList;

static IntList *find_longest_lists(const IntList *lists, size_t count, size_t *result_count)
{
    IntList *result = NULL;
    size_t max_length = 0;
    size_t matches = 0;
    size_t index = 0;
    size_t output_index = 0;

    if (result_count == NULL) {
        return NULL;
    }

    *result_count = 0;

    if (lists == NULL || count == 0) {
        return NULL;
    }

    for (index = 0; index < count; ++index) {
        if (lists[index].length > 0 && lists[index].items == NULL) {
            return NULL;
        }

        if (lists[index].length > max_length) {
            max_length = lists[index].length;
            matches = 1;
        } else if (lists[index].length == max_length) {
            ++matches;
        }
    }

    if (matches > SIZE_MAX / sizeof(*result)) {
        return NULL;
    }

    result = malloc(matches * sizeof(*result));
    if (result == NULL) {
        return NULL;
    }

    for (index = 0; index < count; ++index) {
        if (lists[index].length == max_length) {
            result[output_index] = lists[index];
            ++output_index;
        }
    }

    *result_count = matches;
    return result;
}

static void print_list(const IntList *list)
{
    size_t index = 0;

    if (list == NULL || (list->length > 0 && list->items == NULL)) {
        return;
    }

    putchar('[');
    for (index = 0; index < list->length; ++index) {
        if (index > 0) {
            printf(", ");
        }
        printf("%d", list->items[index]);
    }
    putchar(']');
}

int main(void)
{
    int first[] = {1, 2, 3};
    int second[] = {4, 5, 6, 7, 8};
    int third[] = {9, 10};
    int fourth[] = {11, 12, 13, 14, 15};
    IntList lists[] = {
        {first, sizeof(first) / sizeof(first[0])},
        {second, sizeof(second) / sizeof(second[0])},
        {third, sizeof(third) / sizeof(third[0])},
        {fourth, sizeof(fourth) / sizeof(fourth[0])}
    };
    const size_t list_count = sizeof(lists) / sizeof(lists[0]);
    size_t result_count = 0;
    size_t index = 0;
    IntList *longest = find_longest_lists(lists, list_count, &result_count);

    if (longest == NULL) {
        fputs("Unable to find the longest lists.\n", stderr);
        return EXIT_FAILURE;
    }

    for (index = 0; index < result_count; ++index) {
        print_list(&longest[index]);
        putchar('\n');
    }

    free(longest);
    return EXIT_SUCCESS;
}