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
        return -1;
    }

    node = create_node(value);
    if (node == NULL) {
        return -1;
    }

    if (*tail == NULL) {
        *head = node;
    } else {
        (*tail)->next = node;
    }

    *tail = node;
    return 0;
}

static int contains(const Node *head, int value)
{
    while (head != NULL) {
        if (head->value == value) {
            return 1;
        }

        head = head->next;
    }

    return 0;
}

static int subtract_lists(const Node *left, const Node *right, Node **result)
{
    Node *head = NULL;
    Node *tail = NULL;

    if (result == NULL) {
        return -1;
    }

    *result = NULL;

    while (left != NULL) {
        if (!contains(right, left->value) &&
            append_node(&head, &tail, left->value) != 0) {
            free_list(head);
            return -1;
        }

        left = left->next;
    }

    *result = head;
    return 0;
}

static void print_list(const Node *head)
{
    while (head != NULL) {
        printf("%d", head->value);

        if (head->next != NULL) {
            printf(" ");
        }

        head = head->next;
    }

    printf("\n");
}

static int build_list(const int values[], size_t count, Node **result)
{
    Node *head = NULL;
    Node *tail = NULL;
    size_t i;

    if (result == NULL || (values == NULL && count != 0U)) {
        return -1;
    }

    *result = NULL;

    for (i = 0U; i < count; ++i) {
        if (append_node(&head, &tail, values[i]) != 0) {
            free_list(head);
            return -1;
        }
    }

    *result = head;
    return 0;
}

int main(void)
{
    const int left_values[] = {1, 2, 3, 4, 5, 3};
    const int right_values[] = {2, 4};
    Node *left = NULL;
    Node *right = NULL;
    Node *result = NULL;
    int status = EXIT_FAILURE;

    if (build_list(left_values,
                   sizeof(left_values) / sizeof(left_values[0]),
                   &left) != 0) {
        fprintf(stderr, "Failed to build the first list.\n");
        goto cleanup;
    }

    if (build_list(right_values,
                   sizeof(right_values) / sizeof(right_values[0]),
                   &right) != 0) {
        fprintf(stderr, "Failed to build the second list.\n");
        goto cleanup;
    }

    if (subtract_lists(left, right, &result) != 0) {
        fprintf(stderr, "Failed to subtract the lists.\n");
        goto cleanup;
    }

    print_list(result);
    status = EXIT_SUCCESS;

cleanup:
    free_list(result);
    free_list(right);
    free_list(left);
    return status;
}