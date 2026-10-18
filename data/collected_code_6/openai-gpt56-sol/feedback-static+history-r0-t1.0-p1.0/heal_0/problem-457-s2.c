#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

typedef struct Node {
    int value;
    struct Node *next;
} Node;

typedef struct Sublist {
    Node *head;
    struct Sublist *next;
} Sublist;

static size_t list_length(const Node *head)
{
    size_t length = 0;

    while (head != NULL) {
        ++length;
        head = head->next;
    }

    return length;
}

static const Sublist *find_minimum_length_sublist(const Sublist *sublists)
{
    const Sublist *minimum = NULL;
    size_t minimum_length = 0;

    while (sublists != NULL) {
        size_t current_length = list_length(sublists->head);

        if (minimum == NULL || current_length < minimum_length) {
            minimum = sublists;
            minimum_length = current_length;
        }

        sublists = sublists->next;
    }

    return minimum;
}

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

static void free_nodes(Node *head)
{
    while (head != NULL) {
        Node *next = head->next;
        free(head);
        head = next;
    }
}

static void free_sublists(Sublist *head)
{
    while (head != NULL) {
        Sublist *next = head->next;
        free_nodes(head->head);
        free(head);
        head = next;
    }
}

static int append_value(Node **head, int value)
{
    Node *node;
    Node **cursor;

    if (head == NULL) {
        return 0;
    }

    node = create_node(value);
    if (node == NULL) {
        return 0;
    }

    cursor = head;
    while (*cursor != NULL) {
        cursor = &(*cursor)->next;
    }

    *cursor = node;
    return 1;
}

static int append_sublist(Sublist **head, Node *nodes)
{
    Sublist *sublist;
    Sublist **cursor;

    if (head == NULL) {
        return 0;
    }

    sublist = malloc(sizeof(*sublist));
    if (sublist == NULL) {
        return 0;
    }

    sublist->head = nodes;
    sublist->next = NULL;

    cursor = head;
    while (*cursor != NULL) {
        cursor = &(*cursor)->next;
    }

    *cursor = sublist;
    return 1;
}

static void print_list(const Node *head)
{
    putchar('[');

    while (head != NULL) {
        printf("%d", head->value);
        head = head->next;

        if (head != NULL) {
            printf(", ");
        }
    }

    puts("]");
}

int main(void)
{
    Sublist *sublists = NULL;
    Node *first = NULL;
    Node *second = NULL;
    Node *third = NULL;
    const Sublist *minimum;

    if (!append_value(&first, 1) ||
        !append_value(&first, 2) ||
        !append_value(&first, 3) ||
        !append_value(&second, 4) ||
        !append_value(&third, 5) ||
        !append_value(&third, 6)) {
        free_nodes(first);
        free_nodes(second);
        free_nodes(third);
        return EXIT_FAILURE;
    }

    if (!append_sublist(&sublists, first)) {
        free_nodes(first);
        free_nodes(second);
        free_nodes(third);
        return EXIT_FAILURE;
    }
    first = NULL;

    if (!append_sublist(&sublists, second)) {
        free_sublists(sublists);
        free_nodes(second);
        free_nodes(third);
        return EXIT_FAILURE;
    }
    second = NULL;

    if (!append_sublist(&sublists, third)) {
        free_sublists(sublists);
        free_nodes(third);
        return EXIT_FAILURE;
    }
    third = NULL;

    minimum = find_minimum_length_sublist(sublists);
    if (minimum == NULL) {
        free_sublists(sublists);
        return EXIT_FAILURE;
    }

    print_list(minimum->head);
    free_sublists(sublists);

    return EXIT_SUCCESS;
}