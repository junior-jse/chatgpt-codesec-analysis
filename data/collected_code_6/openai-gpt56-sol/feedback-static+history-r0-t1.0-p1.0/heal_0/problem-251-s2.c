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

static int append(Node **head, Node **tail, int value)
{
    Node *node;

    if (head == NULL || tail == NULL) {
        return 0;
    }

    node = create_node(value);
    if (node == NULL) {
        return 0;
    }

    if (*tail == NULL) {
        *head = node;
    } else {
        (*tail)->next = node;
    }

    *tail = node;
    return 1;
}

static int insert_before_each(Node **head, int value)
{
    Node *current;
    Node *new_head = NULL;
    Node *new_tail = NULL;

    if (head == NULL) {
        return 0;
    }

    current = *head;
    while (current != NULL) {
        Node *inserted = create_node(value);

        if (inserted == NULL) {
            free_list(new_head);
            return 0;
        }

        if (new_tail == NULL) {
            new_head = inserted;
        } else {
            new_tail->next = inserted;
        }

        new_tail = current;
        current = current->next;
        inserted->next = new_tail;
        new_tail->next = current;
    }

    *head = new_head;
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

    printf("\n");
}

int main(void)
{
    Node *head = NULL;
    Node *tail = NULL;
    const int values[] = {1, 2, 3, 4, 5};
    const size_t count = sizeof(values) / sizeof(values[0]);
    size_t index;

    for (index = 0; index < count; ++index) {
        if (!append(&head, &tail, values[index])) {
            free_list(head);
            return EXIT_FAILURE;
        }
    }

    if (!insert_before_each(&head, 0)) {
        free_list(head);
        return EXIT_FAILURE;
    }

    print_list(head);
    free_list(head);
    return EXIT_SUCCESS;
}