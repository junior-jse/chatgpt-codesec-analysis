#include <stdio.h>
#include <stdlib.h>

struct Node {
    int value;
    struct Node *next;
};

static int list_sum(const struct Node *node)
{
    if (node == NULL) {
        return 0;
    }

    return node->value + list_sum(node->next);
}

static void free_list(struct Node *head)
{
    while (head != NULL) {
        struct Node *next = head->next;
        free(head);
        head = next;
    }
}

static int append_node(struct Node **head, struct Node **tail, int value)
{
    struct Node *node = malloc(sizeof(*node));

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

int main(void)
{
    struct Node *head = NULL;
    struct Node *tail = NULL;
    const int values[] = {1, 2, 3, 4, 5};
    const size_t count = sizeof(values) / sizeof(values[0]);

    for (size_t i = 0; i < count; ++i) {
        if (!append_node(&head, &tail, values[i])) {
            fputs("Memory allocation failed\n", stderr);
            free_list(head);
            return EXIT_FAILURE;
        }
    }

    printf("%d\n", list_sum(head));
    free_list(head);

    return EXIT_SUCCESS;
}