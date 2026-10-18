#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

typedef struct Node {
    int value;
    struct Node *next;
} Node;

typedef struct {
    Node *head;
    Node *tail;
} List;

static Node *create_node(int value)
{
    Node *node = malloc(sizeof(*node));

    if (node == NULL) {
        return NULL;
    }

    node->value = value;
    node->next = NULL;
    return node;
}

static int append_node(List *list, int value)
{
    Node *node;

    if (list == NULL) {
        return 0;
    }

    node = create_node(value);
    if (node == NULL) {
        return 0;
    }

    if (list->tail == NULL) {
        list->head = node;
    } else {
        list->tail->next = node;
    }

    list->tail = node;
    return 1;
}

static void free_list(Node *head)
{
    while (head != NULL) {
        Node *next = head->next;
        free(head);
        head = next;
    }
}

static void free_lists(List *lists, size_t count)
{
    size_t i;

    if (lists == NULL) {
        return;
    }

    for (i = 0; i < count; ++i) {
        free_list(lists[i].head);
    }

    free(lists);
}

static int split_every_nth(Node *head, size_t n, List **result, size_t *count)
{
    Node *current;
    List *lists;
    size_t node_count = 0;
    size_t list_count;
    size_t list_index = 0;
    size_t position = 0;

    if (result == NULL || count == NULL || n == 0) {
        return 0;
    }

    *result = NULL;
    *count = 0;

    for (current = head; current != NULL; current = current->next) {
        if (node_count == SIZE_MAX) {
            return 0;
        }
        ++node_count;
    }

    if (node_count == 0) {
        return 1;
    }

    list_count = node_count / n;
    if (node_count % n != 0) {
        ++list_count;
    }

    if (list_count > SIZE_MAX / sizeof(*lists)) {
        return 0;
    }

    lists = calloc(list_count, sizeof(*lists));
    if (lists == NULL) {
        return 0;
    }

    current = head;
    while (current != NULL) {
        if (!append_node(&lists[list_index], current->value)) {
            free_lists(lists, list_count);
            return 0;
        }

        ++position;
        if (position == n) {
            position = 0;
            ++list_index;
        }

        current = current->next;
    }

    *result = lists;
    *count = list_count;
    return 1;
}

static void print_lists(const List *lists, size_t count)
{
    size_t i;

    for (i = 0; i < count; ++i) {
        const Node *current = lists[i].head;

        printf("List %zu:", i + 1);
        while (current != NULL) {
            printf(" %d", current->value);
            current = current->next;
        }
        putchar('\n');
    }
}

int main(void)
{
    const int values[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    const size_t value_count = sizeof(values) / sizeof(values[0]);
    const size_t n = 3;
    List source = {NULL, NULL};
    List *split_lists = NULL;
    size_t split_count = 0;
    size_t i;
    int status = EXIT_FAILURE;

    for (i = 0; i < value_count; ++i) {
        if (!append_node(&source, values[i])) {
            fprintf(stderr, "Failed to allocate source list\n");
            goto cleanup;
        }
    }

    if (!split_every_nth(source.head, n, &split_lists, &split_count)) {
        fprintf(stderr, "Failed to split list\n");
        goto cleanup;
    }

    print_lists(split_lists, split_count);
    status = EXIT_SUCCESS;

cleanup:
    free_lists(split_lists, split_count);
    free_list(source.head);
    return status;
}