#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int value;
    struct Node *next;
} Node;

int shift_first_to_end(Node **head)
{
    Node *first;
    Node *last;

    if (head == NULL) {
        return -1;
    }

    if (*head == NULL || (*head)->next == NULL) {
        return 0;
    }

    first = *head;
    *head = first->next;
    first->next = NULL;

    last = *head;
    while (last->next != NULL) {
        last = last->next;
    }

    last->next = first;
    return 0;
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

static void free_list(Node **head)
{
    Node *current;

    if (head == NULL) {
        return;
    }

    current = *head;
    while (current != NULL) {
        Node *next = current->next;
        free(current);
        current = next;
    }

    *head = NULL;
}

static void print_list(const Node *head)
{
    const Node *current = head;

    while (current != NULL) {
        printf("%d%s", current->value, current->next != NULL ? " " : "\n");
        current = current->next;
    }

    if (head == NULL) {
        putchar('\n');
    }
}

int main(void)
{
    const int values[] = {1, 2, 3, 4, 5};
    const size_t count = sizeof(values) / sizeof(values[0]);
    Node *head = NULL;
    Node *tail = NULL;

    for (size_t i = 0; i < count; ++i) {
        Node *node = create_node(values[i]);

        if (node == NULL) {
            fputs("Failed to allocate memory.\n", stderr);
            free_list(&head);
            return EXIT_FAILURE;
        }

        if (head == NULL) {
            head = node;
        } else {
            tail->next = node;
        }

        tail = node;
    }

    if (shift_first_to_end(&head) != 0) {
        fputs("Failed to shift list.\n", stderr);
        free_list(&head);
        return EXIT_FAILURE;
    }

    print_list(head);
    free_list(&head);

    return EXIT_SUCCESS;
}