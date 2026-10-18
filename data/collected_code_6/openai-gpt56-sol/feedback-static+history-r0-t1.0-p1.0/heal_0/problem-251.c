#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int value;
    struct Node *next;
} Node;

static Node *create_node(int value)
{
    Node *node = malloc(sizeof(*node));

    if (node != NULL) {
        node->value = value;
        node->next = NULL;
    }

    return node;
}

static int append(Node **head, int value)
{
    Node *node;
    Node **link;

    if (head == NULL) {
        return 0;
    }

    node = create_node(value);
    if (node == NULL) {
        return 0;
    }

    link = head;
    while (*link != NULL) {
        link = &(*link)->next;
    }

    *link = node;
    return 1;
}

static int insert_before_each(Node **head, int value)
{
    Node *current;

    if (head == NULL) {
        return 0;
    }

    current = *head;
    while (current != NULL) {
        Node *inserted = create_node(value);

        if (inserted == NULL) {
            return 0;
        }

        inserted->next = current;
        *head = inserted;
        head = &current->next;
        current = current->next;
    }

    return 1;
}

static void print_list(const Node *head)
{
    const Node *current = head;

    while (current != NULL) {
        printf("%d", current->value);
        current = current->next;

        if (current != NULL) {
            printf(" ");
        }
    }

    putchar('\n');
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

int main(void)
{
    Node *list = NULL;
    int status = EXIT_FAILURE;

    if (!append(&list, 1) ||
        !append(&list, 2) ||
        !append(&list, 3)) {
        fputs("Failed to create the list.\n", stderr);
        goto cleanup;
    }

    if (!insert_before_each(&list, 0)) {
        fputs("Failed to insert all elements.\n", stderr);
        goto cleanup;
    }

    print_list(list);
    status = EXIT_SUCCESS;

cleanup:
    free_list(&list);
    return status;
}