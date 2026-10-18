#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int value;
    struct Node *next;
} Node;

static int append_node(Node **head, Node **tail, int value)
{
    Node *node = malloc(sizeof(*node));

    if (node == NULL) {
        return 0;
    }

    node->value = value;
    node->next = NULL;

    if (*tail == NULL) {
        *head = node;
    } else {
        (*tail)->next = node;
    }

    *tail = node;
    return 1;
}

static void remove_even_numbers(Node **head)
{
    Node **current;

    if (head == NULL) {
        return;
    }

    current = head;

    while (*current != NULL) {
        if ((*current)->value % 2 == 0) {
            Node *removed = *current;
            *current = removed->next;
            free(removed);
        } else {
            current = &(*current)->next;
        }
    }
}

static void print_list(const Node *head)
{
    for (const Node *current = head; current != NULL; current = current->next) {
        printf("%d%s", current->value, current->next == NULL ? "\n" : " ");
    }

    if (head == NULL) {
        putchar('\n');
    }
}

static void free_list(Node **head)
{
    if (head == NULL) {
        return;
    }

    while (*head != NULL) {
        Node *removed = *head;
        *head = removed->next;
        free(removed);
    }
}

int main(void)
{
    const int values[] = {1, 2, 3, 4, 5, 6, 7, 8};
    const size_t count = sizeof(values) / sizeof(values[0]);
    Node *head = NULL;
    Node *tail = NULL;

    for (size_t i = 0; i < count; ++i) {
        if (!append_node(&head, &tail, values[i])) {
            free_list(&head);
            fputs("Memory allocation failed.\n", stderr);
            return EXIT_FAILURE;
        }
    }

    remove_even_numbers(&head);
    print_list(head);
    free_list(&head);

    return EXIT_SUCCESS;
}