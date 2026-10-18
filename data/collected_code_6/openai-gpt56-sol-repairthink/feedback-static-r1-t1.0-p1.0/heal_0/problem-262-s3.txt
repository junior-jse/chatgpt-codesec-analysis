#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Node {
    int value;
    struct Node *next;
} Node;

static void free_list(Node *head)
{
    while (head != NULL) {
        Node *next = head->next;
        free(head);
        head = next;
    }
}

static bool append_node(Node **head, Node **tail, int value)
{
    Node *node = malloc(sizeof(*node));

    if (node == NULL) {
        return false;
    }

    node->value = value;
    node->next = NULL;

    if (*tail == NULL) {
        *head = node;
    } else {
        (*tail)->next = node;
    }

    *tail = node;
    return true;
}

static bool split_list(Node *head, size_t first_length,
                       Node **first, Node **second)
{
    Node *current = head;
    Node *previous = NULL;
    size_t index = 0;

    if (first == NULL || second == NULL) {
        return false;
    }

    *first = NULL;
    *second = NULL;

    while (current != NULL && index < first_length) {
        previous = current;
        current = current->next;
        ++index;
    }

    if (index != first_length) {
        return false;
    }

    *first = head;
    *second = current;

    if (previous != NULL) {
        previous->next = NULL;
    }

    return true;
}

static void print_list(const Node *head)
{
    while (head != NULL) {
        printf("%d", head->value);
        head = head->next;

        if (head != NULL) {
            printf(" ");
        }
    }

    printf("\n");
}

int main(void)
{
    Node *list = NULL;
    Node *tail = NULL;
    Node *first = NULL;
    Node *second = NULL;
    size_t count;
    size_t first_length;

    if (scanf("%zu", &count) != 1) {
        fprintf(stderr, "Invalid list length\n");
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < count; ++i) {
        int value;

        if (scanf("%d", &value) != 1) {
            fprintf(stderr, "Invalid list element\n");
            free_list(list);
            return EXIT_FAILURE;
        }

        if (!append_node(&list, &tail, value)) {
            fprintf(stderr, "Memory allocation failed\n");
            free_list(list);
            return EXIT_FAILURE;
        }
    }

    if (scanf("%zu", &first_length) != 1) {
        fprintf(stderr, "Invalid split length\n");
        free_list(list);
        return EXIT_FAILURE;
    }

    if (!split_list(list, first_length, &first, &second)) {
        fprintf(stderr, "Split length exceeds list length\n");
        free_list(list);
        return EXIT_FAILURE;
    }

    print_list(first);
    print_list(second);

    free_list(first);
    free_list(second);

    return EXIT_SUCCESS;
}