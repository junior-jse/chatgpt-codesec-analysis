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
        return -1;
    }

    node = create_node(value);
    if (node == NULL) {
        return -1;
    }

    link = head;
    while (*link != NULL) {
        link = &(*link)->next;
    }

    *link = node;
    return 0;
}

static int replace_last_with_list(Node **head, Node **replacement)
{
    Node *current;
    Node *tail;

    if (head == NULL || replacement == NULL) {
        return -1;
    }

    if (*head == NULL) {
        *head = *replacement;
        *replacement = NULL;
        return 0;
    }

    current = *head;
    if (current->next == NULL) {
        free(current);
        *head = *replacement;
        *replacement = NULL;
        return 0;
    }

    while (current->next->next != NULL) {
        current = current->next;
    }

    tail = current->next;
    current->next = *replacement;
    *replacement = NULL;
    free(tail);
    return 0;
}

static void print_list(const Node *head)
{
    const Node *current = head;

    while (current != NULL) {
        printf("%d", current->value);
        if (current->next != NULL) {
            printf(" ");
        }
        current = current->next;
    }

    putchar('\n');
}

int main(void)
{
    Node *list = NULL;
    Node *replacement = NULL;

    if (append(&list, 1) != 0 ||
        append(&list, 2) != 0 ||
        append(&list, 3) != 0 ||
        append(&replacement, 4) != 0 ||
        append(&replacement, 5) != 0) {
        free_list(list);
        free_list(replacement);
        return EXIT_FAILURE;
    }

    if (replace_last_with_list(&list, &replacement) != 0) {
        free_list(list);
        free_list(replacement);
        return EXIT_FAILURE;
    }

    print_list(list);
    free_list(list);
    free_list(replacement);
    return EXIT_SUCCESS;
}