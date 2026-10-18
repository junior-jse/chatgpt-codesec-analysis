#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int value;
    struct Node *next;
} Node;

typedef struct {
    Node **lists;
    size_t count;
} SplitResult;

static Node *node_create(int value)
{
    Node *node = malloc(sizeof(*node));

    if (node == NULL) {
        return NULL;
    }

    node->value = value;
    node->next = NULL;
    return node;
}

static void list_free(Node *head)
{
    while (head != NULL) {
        Node *next = head->next;
        free(head);
        head = next;
    }
}

static void split_result_free(SplitResult *result)
{
    size_t i;

    if (result == NULL) {
        return;
    }

    for (i = 0; i < result->count; ++i) {
        list_free(result->lists[i]);
    }

    free(result->lists);
    result->lists = NULL;
    result->count = 0;
}

static int list_append(Node **head, Node **tail, int value)
{
    Node *node;

    if (head == NULL || tail == NULL) {
        return 0;
    }

    node = node_create(value);
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

static int split_every_nth(Node *head, size_t n, SplitResult *result)
{
    Node *current;
    size_t length = 0;
    size_t group_count;
    size_t group_index = 0;
    size_t position = 0;

    if (result == NULL || n == 0) {
        return 0;
    }

    result->lists = NULL;
    result->count = 0;

    for (current = head; current != NULL; current = current->next) {
        if (length == SIZE_MAX) {
            return 0;
        }
        ++length;
    }

    if (length == 0) {
        return 1;
    }

    group_count = length / n + (length % n != 0);

    if (group_count > SIZE_MAX / sizeof(*result->lists)) {
        return 0;
    }

    result->lists = calloc(group_count, sizeof(*result->lists));
    if (result->lists == NULL) {
        return 0;
    }

    result->count = group_count;
    current = head;

    while (current != NULL) {
        Node *next = current->next;

        if (position == 0) {
            result->lists[group_index] = current;
        }

        ++position;

        if (position == n) {
            current->next = NULL;
            position = 0;
            ++group_index;
        }

        current = next;
    }

    return 1;
}

static void print_list(const Node *head)
{
    const Node *current = head;

    putchar('[');

    while (current != NULL) {
        printf("%d", current->value);
        current = current->next;

        if (current != NULL) {
            printf(", ");
        }
    }

    puts("]");
}

int main(void)
{
    const int values[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    const size_t value_count = sizeof(values) / sizeof(values[0]);
    const size_t n = 3;
    Node *head = NULL;
    Node *tail = NULL;
    SplitResult result = {NULL, 0};
    size_t i;

    for (i = 0; i < value_count; ++i) {
        if (!list_append(&head, &tail, values[i])) {
            fprintf(stderr, "Failed to allocate list node.\n");
            list_free(head);
            return EXIT_FAILURE;
        }
    }

    if (!split_every_nth(head, n, &result)) {
        fprintf(stderr, "Failed to split the list.\n");
        list_free(head);
        return EXIT_FAILURE;
    }

    head = NULL;

    for (i = 0; i < result.count; ++i) {
        print_list(result.lists[i]);
    }

    split_result_free(&result);
    return EXIT_SUCCESS;
}