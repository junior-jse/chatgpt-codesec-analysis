#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

typedef struct List {
    Node *head;
} List;

static Node *create_node(int value)
{
    Node *node = malloc(sizeof(*node));
    if (node == NULL) {
        return NULL;
    }
    node->data = value;
    node->next = NULL;
    return node;
}

static int append_node(List *list, int value)
{
    Node *node;
    Node *current;

    if (list == NULL) {
        return 0;
    }

    node = create_node(value);
    if (node == NULL) {
        return 0;
    }

    if (list->head == NULL) {
        list->head = node;
        return 1;
    }

    current = list->head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = node;
    return 1;
}

static void reverse_list(List *list)
{
    Node *previous = NULL;
    Node *current;
    Node *next;

    if (list == NULL) {
        return;
    }

    current = list->head;
    while (current != NULL) {
        next = current->next;
        current->next = previous;
        previous = current;
        current = next;
    }
    list->head = previous;
}

static void reverse_all_lists(List *lists, size_t count)
{
    size_t i;

    if (lists == NULL) {
        return;
    }

    for (i = 0; i < count; ++i) {
        reverse_list(&lists[i]);
    }
}

static void print_list(const List *list)
{
    const Node *current;

    if (list == NULL) {
        return;
    }

    current = list->head;
    while (current != NULL) {
        printf("%d", current->data);
        if (current->next != NULL) {
            printf(" -> ");
        }
        current = current->next;
    }
    printf("\n");
}

static void free_list(List *list)
{
    Node *current;
    Node *next;

    if (list == NULL) {
        return;
    }

    current = list->head;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
    list->head = NULL;
}

static void free_all_lists(List *lists, size_t count)
{
    size_t i;

    if (lists == NULL) {
        return;
    }

    for (i = 0; i < count; ++i) {
        free_list(&lists[i]);
    }
    free(lists);
}

int main(void)
{
    List *lists;
    size_t count = 3;
    size_t i;
    int values[][5] = {
        {1, 2, 3, 4, 5},
        {10, 20, 30, 0, 0},
        {7, 8, 9, 11, 13}
    };
    const size_t lengths[] = {5, 3, 5};

    lists = calloc(count, sizeof(*lists));
    if (lists == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    for (i = 0; i < count; ++i) {
        size_t j;
        for (j = 0; j < lengths[i]; ++j) {
            if (!append_node(&lists[i], values[i][j])) {
                fprintf(stderr, "Failed to append node\n");
                free_all_lists(lists, count);
                return EXIT_FAILURE;
            }
        }
    }

    printf("Original lists:\n");
    for (i = 0; i < count; ++i) {
        print_list(&lists[i]);
    }

    reverse_all_lists(lists, count);

    printf("Reversed lists:\n");
    for (i = 0; i < count; ++i) {
        print_list(&lists[i]);
    }

    free_all_lists(lists, count);
    return EXIT_SUCCESS;
}