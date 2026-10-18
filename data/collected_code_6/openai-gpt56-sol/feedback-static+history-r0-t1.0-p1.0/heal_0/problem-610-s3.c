#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

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

static int append(Node **head, int value)
{
    Node *node;
    Node *current;

    if (head == NULL) {
        return 0;
    }

    node = create_node(value);
    if (node == NULL) {
        return 0;
    }

    if (*head == NULL) {
        *head = node;
        return 1;
    }

    current = *head;
    while (current->next != NULL) {
        current = current->next;
    }

    current->next = node;
    return 1;
}

static int remove_kth(Node **head, size_t k)
{
    Node *current;
    Node *removed;
    size_t index;

    if (head == NULL || *head == NULL || k == 0U) {
        return 0;
    }

    if (k == 1U) {
        removed = *head;
        *head = removed->next;
        free(removed);
        return 1;
    }

    current = *head;
    for (index = 1U; index < k - 1U; ++index) {
        if (current->next == NULL) {
            return 0;
        }
        current = current->next;
    }

    removed = current->next;
    if (removed == NULL) {
        return 0;
    }

    current->next = removed->next;
    free(removed);
    return 1;
}

static void print_list(const Node *head)
{
    const Node *current = head;

    while (current != NULL) {
        printf("%d", current->value);
        if (current->next != NULL) {
            putchar(' ');
        }
        current = current->next;
    }

    putchar('\n');
}

static void free_list(Node **head)
{
    Node *current;
    Node *next;

    if (head == NULL) {
        return;
    }

    current = *head;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }

    *head = NULL;
}

int main(void)
{
    Node *head = NULL;
    size_t count;
    size_t k;
    size_t index;
    int value;
    int status = EXIT_FAILURE;

    if (scanf("%zu", &count) != 1) {
        goto cleanup;
    }

    for (index = 0U; index < count; ++index) {
        if (scanf("%d", &value) != 1) {
            goto cleanup;
        }

        if (!append(&head, value)) {
            goto cleanup;
        }
    }

    if (scanf("%zu", &k) != 1) {
        goto cleanup;
    }

    if (!remove_kth(&head, k)) {
        goto cleanup;
    }

    print_list(head);
    status = EXIT_SUCCESS;

cleanup:
    free_list(&head);
    return status;
}