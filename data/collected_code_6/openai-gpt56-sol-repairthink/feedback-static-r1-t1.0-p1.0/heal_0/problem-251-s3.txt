#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int value;
    struct Node *next;
} Node;

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

static void free_list(Node *head)
{
    while (head != NULL) {
        Node *next = head->next;
        free(head);
        head = next;
    }
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

static int print_list(const Node *head)
{
    while (head != NULL) {
        if (printf("%d%s", head->value, head->next != NULL ? " " : "\n") < 0) {
            return 0;
        }
        head = head->next;
    }

    return 1;
}

int main(void)
{
    Node *head = NULL;
    int values[] = {1, 2, 3, 4, 5};
    size_t count = sizeof(values) / sizeof(values[0]);
    size_t index;
    int status = EXIT_SUCCESS;

    for (index = 0; index < count; ++index) {
        if (!append(&head, values[index])) {
            fprintf(stderr, "Unable to allocate list node.\n");
            free_list(head);
            return EXIT_FAILURE;
        }
    }

    if (!insert_before_each(&head, 0)) {
        fprintf(stderr, "Unable to insert all list elements.\n");
        status = EXIT_FAILURE;
    } else if (!print_list(head)) {
        fprintf(stderr, "Unable to write output.\n");
        status = EXIT_FAILURE;
    }

    free_list(head);
    return status;
}