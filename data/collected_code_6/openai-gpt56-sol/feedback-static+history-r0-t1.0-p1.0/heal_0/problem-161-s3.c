#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Node {
    int value;
    struct Node *next;
} Node;

static bool contains(const Node *list, int value)
{
    while (list != NULL) {
        if (list->value == value) {
            return true;
        }
        list = list->next;
    }
    return false;
}

static int append(Node **head, Node **tail, int value)
{
    Node *node;

    if (head == NULL || tail == NULL) {
        return -1;
    }

    node = malloc(sizeof(*node));
    if (node == NULL) {
        return -1;
    }

    node->value = value;
    node->next = NULL;

    if (*tail == NULL) {
        *head = node;
    } else {
        (*tail)->next = node;
    }

    *tail = node;
    return 0;
}

static void remove_present(Node **list, const Node *other)
{
    Node **current;

    if (list == NULL) {
        return;
    }

    current = list;
    while (*current != NULL) {
        if (contains(other, (*current)->value)) {
            Node *removed = *current;
            *current = removed->next;
            free(removed);
        } else {
            current = &(*current)->next;
        }
    }
}

static void print_list(const Node *list)
{
    while (list != NULL) {
        printf("%d", list->value);
        if (list->next != NULL) {
            putchar(' ');
        }
        list = list->next;
    }
    putchar('\n');
}

static void free_list(Node **list)
{
    Node *current;

    if (list == NULL) {
        return;
    }

    current = *list;
    while (current != NULL) {
        Node *next = current->next;
        free(current);
        current = next;
    }

    *list = NULL;
}

int main(void)
{
    const int first_values[] = {1, 2, 3, 4, 5, 2, 6};
    const int second_values[] = {2, 4, 7};
    Node *first = NULL;
    Node *first_tail = NULL;
    Node *second = NULL;
    Node *second_tail = NULL;
    size_t i;
    int status = EXIT_FAILURE;

    for (i = 0; i < sizeof(first_values) / sizeof(first_values[0]); ++i) {
        if (append(&first, &first_tail, first_values[i]) != 0) {
            goto cleanup;
        }
    }

    for (i = 0; i < sizeof(second_values) / sizeof(second_values[0]); ++i) {
        if (append(&second, &second_tail, second_values[i]) != 0) {
            goto cleanup;
        }
    }

    remove_present(&first, second);
    print_list(first);
    status = EXIT_SUCCESS;

cleanup:
    free_list(&first);
    free_list(&second);
    return status;
}