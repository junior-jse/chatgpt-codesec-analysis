#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int value;
    struct Node *next;
} Node;

static int append(Node **head, int value)
{
    Node *node;
    Node **current;

    if (head == NULL) {
        return EINVAL;
    }

    node = malloc(sizeof(*node));
    if (node == NULL) {
        return ENOMEM;
    }

    node->value = value;
    node->next = NULL;

    current = head;
    while (*current != NULL) {
        current = &(*current)->next;
    }
    *current = node;

    return 0;
}

static int remove_kth(Node **head, size_t k, int *removed_value)
{
    Node **current;
    Node *removed;
    size_t position;

    if (head == NULL || k == 0U) {
        return EINVAL;
    }

    current = head;
    for (position = 1U; position < k; ++position) {
        if (*current == NULL) {
            return ERANGE;
        }
        current = &(*current)->next;
    }

    if (*current == NULL) {
        return ERANGE;
    }

    removed = *current;
    *current = removed->next;

    if (removed_value != NULL) {
        *removed_value = removed->value;
    }

    free(removed);
    return 0;
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

static void free_list(Node **head)
{
    Node *current;

    if (head == NULL) {
        return;
    }

    while (*head != NULL) {
        current = *head;
        *head = current->next;
        free(current);
    }
}

int main(void)
{
    Node *list = NULL;
    const int values[] = {10, 20, 30, 40, 50};
    const size_t count = sizeof(values) / sizeof(values[0]);
    const size_t k = 3U;
    size_t i;
    int status;

    for (i = 0U; i < count; ++i) {
        status = append(&list, values[i]);
        if (status != 0) {
            fprintf(stderr, "Failed to create list: error %d\n", status);
            free_list(&list);
            return EXIT_FAILURE;
        }
    }

    status = remove_kth(&list, k, NULL);
    if (status != 0) {
        fprintf(stderr, "Failed to remove element %zu: error %d\n", k, status);
        free_list(&list);
        return EXIT_FAILURE;
    }

    print_list(list);
    free_list(&list);

    return EXIT_SUCCESS;
}