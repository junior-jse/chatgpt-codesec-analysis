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

static int append_node(Node **head, Node **tail, int value)
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

static int split_list(Node *head, size_t first_length,
                      Node **first, Node **second)
{
    Node *current;
    size_t index;

    if (first == NULL || second == NULL) {
        return 0;
    }

    *first = NULL;
    *second = NULL;

    if (first_length == 0U) {
        *second = head;
        return 1;
    }

    current = head;
    for (index = 1U; index < first_length; ++index) {
        if (current == NULL) {
            return 0;
        }
        current = current->next;
    }

    if (current == NULL) {
        return 0;
    }

    *first = head;
    *second = current->next;
    current->next = NULL;
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
    Node *first = NULL;
    Node *second = NULL;
    size_t count;
    size_t first_length;
    size_t index;
    int value;

    if (scanf("%zu", &count) != 1) {
        fprintf(stderr, "Invalid list length\n");
        return EXIT_FAILURE;
    }

    for (index = 0U; index < count; ++index) {
        if (scanf("%d", &value) != 1) {
            fprintf(stderr, "Invalid list element\n");
            free_list(head);
            return EXIT_FAILURE;
        }

        if (!append_node(&head, &tail, value)) {
            fprintf(stderr, "Memory allocation failed\n");
            free_list(head);
            return EXIT_FAILURE;
        }
    }

    if (scanf("%zu", &first_length) != 1) {
        fprintf(stderr, "Invalid split length\n");
        free_list(head);
        return EXIT_FAILURE;
    }

    if (first_length > count ||
        !split_list(head, first_length, &first, &second)) {
        fprintf(stderr, "Split length exceeds list length\n");
        free_list(head);
        return EXIT_FAILURE;
    }

    print_list(first);
    print_list(second);

    free_list(first);
    free_list(second);
    return EXIT_SUCCESS;
}