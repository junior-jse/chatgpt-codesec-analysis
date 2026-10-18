#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

typedef struct List {
    void *data;
    struct List *next;
} List;

static int count_lists_squared(const List *lists, uintmax_t *result)
{
    uintmax_t count = 0;

    if (result == NULL) {
        return 0;
    }

    while (lists != NULL) {
        if (count == UINTMAX_MAX) {
            return 0;
        }
        ++count;
        lists = lists->next;
    }

    if (count != 0 && count > UINTMAX_MAX / count) {
        return 0;
    }

    *result = count * count;
    return 1;
}

static void free_list(List *list)
{
    while (list != NULL) {
        List *next = list->next;
        free(list);
        list = next;
    }
}

static int append_list(List **head, List **tail, void *data)
{
    List *node;

    if (head == NULL || tail == NULL) {
        return 0;
    }

    node = malloc(sizeof(*node));
    if (node == NULL) {
        return 0;
    }

    node->data = data;
    node->next = NULL;

    if (*tail == NULL) {
        *head = node;
    } else {
        (*tail)->next = node;
    }

    *tail = node;
    return 1;
}

int main(void)
{
    List *lists = NULL;
    List *tail = NULL;
    uintmax_t squared_count;

    for (size_t i = 0; i < 4; ++i) {
        if (!append_list(&lists, &tail, NULL)) {
            free_list(lists);
            fputs("Failed to allocate memory.\n", stderr);
            return EXIT_FAILURE;
        }
    }

    if (!count_lists_squared(lists, &squared_count)) {
        free_list(lists);
        fputs("Failed to calculate the squared count.\n", stderr);
        return EXIT_FAILURE;
    }

    printf("%" PRIuMAX "\n", squared_count);
    free_list(lists);

    return EXIT_SUCCESS;
}