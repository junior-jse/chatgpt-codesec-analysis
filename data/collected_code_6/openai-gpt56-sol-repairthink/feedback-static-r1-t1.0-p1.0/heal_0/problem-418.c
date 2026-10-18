#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int value;
    struct Node *next;
} Node;

static size_t list_length(const Node *head)
{
    size_t length = 0;

    while (head != NULL) {
        ++length;
        head = head->next;
    }

    return length;
}

static const Node *find_longest_sublist(const Node *const lists[],
                                        size_t list_count,
                                        size_t *max_length)
{
    const Node *longest = NULL;
    size_t longest_length = 0;

    if (max_length == NULL || (lists == NULL && list_count != 0)) {
        return NULL;
    }

    for (size_t i = 0; i < list_count; ++i) {
        size_t current_length = list_length(lists[i]);

        if (current_length > longest_length) {
            longest = lists[i];
            longest_length = current_length;
        }
    }

    *max_length = longest_length;
    return longest;
}

static Node *create_list(const int values[], size_t count)
{
    Node *head = NULL;
    Node *tail = NULL;

    if (values == NULL && count != 0) {
        return NULL;
    }

    for (size_t i = 0; i < count; ++i) {
        Node *node = malloc(sizeof(*node));

        if (node == NULL) {
            while (head != NULL) {
                Node *next = head->next;
                free(head);
                head = next;
            }
            return NULL;
        }

        node->value = values[i];
        node->next = NULL;

        if (tail == NULL) {
            head = node;
        } else {
            tail->next = node;
        }

        tail = node;
    }

    return head;
}

static void free_list(Node *head)
{
    while (head != NULL) {
        Node *next = head->next;
        free(head);
        head = next;
    }
}

static int print_list(const Node *head)
{
    if (printf("[") < 0) {
        return -1;
    }

    while (head != NULL) {
        if (printf("%d%s", head->value, head->next != NULL ? ", " : "") < 0) {
            return -1;
        }
        head = head->next;
    }

    return printf("]\n") < 0 ? -1 : 0;
}

int main(void)
{
    const int values1[] = {1, 2, 3};
    const int values2[] = {4, 5, 6, 7, 8};
    const int values3[] = {9, 10, 11, 12};
    Node *lists[3] = {NULL, NULL, NULL};
    size_t max_length = 0;
    int status = EXIT_FAILURE;

    lists[0] = create_list(values1, sizeof(values1) / sizeof(values1[0]));
    if (lists[0] == NULL) {
        goto cleanup;
    }

    lists[1] = create_list(values2, sizeof(values2) / sizeof(values2[0]));
    if (lists[1] == NULL) {
        goto cleanup;
    }

    lists[2] = create_list(values3, sizeof(values3) / sizeof(values3[0]));
    if (lists[2] == NULL) {
        goto cleanup;
    }

    const Node *longest = find_longest_sublist(
        (const Node *const *)lists,
        sizeof(lists) / sizeof(lists[0]),
        &max_length
    );

    if (longest == NULL || printf("Maximum length: %zu\n", max_length) < 0 ||
        print_list(longest) != 0) {
        goto cleanup;
    }

    status = EXIT_SUCCESS;

cleanup:
    for (size_t i = 0; i < sizeof(lists) / sizeof(lists[0]); ++i) {
        free_list(lists[i]);
    }

    return status;
}