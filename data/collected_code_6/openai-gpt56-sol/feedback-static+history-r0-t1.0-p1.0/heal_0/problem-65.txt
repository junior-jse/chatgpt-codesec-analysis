#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int value;
    struct Node *next;
} Node;

static int list_sum(const Node *node)
{
    return node == NULL ? 0 : node->value + list_sum(node->next);
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

static void free_list(Node *head)
{
    while (head != NULL) {
        Node *next = head->next;
        free(head);
        head = next;
    }
}

int main(void)
{
    const int values[] = {1, 2, 3, 4, 5};
    const size_t count = sizeof(values) / sizeof(values[0]);
    Node *head = NULL;
    Node **tail = &head;

    for (size_t i = 0; i < count; ++i) {
        Node *node = create_node(values[i]);

        if (node == NULL) {
            free_list(head);
            fputs("Memory allocation failed\n", stderr);
            return EXIT_FAILURE;
        }

        *tail = node;
        tail = &node->next;
    }

    printf("Sum: %d\n", list_sum(head));
    free_list(head);

    return EXIT_SUCCESS;
}