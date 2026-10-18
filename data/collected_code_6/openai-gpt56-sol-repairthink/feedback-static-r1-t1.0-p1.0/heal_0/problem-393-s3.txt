#include <stddef.h>
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
        if (length == SIZE_MAX) {
            fprintf(stderr, "List length overflow\n");
            exit(EXIT_FAILURE);
        }
        ++length;
        head = head->next;
    }

    return length;
}

static const Node *find_longest_list(const Node *const lists[], size_t count,
                                     size_t *longest_index,
                                     size_t *longest_length)
{
    const Node *longest = NULL;
    size_t best_index = 0;
    size_t best_length = 0;

    if (lists == NULL || longest_index == NULL || longest_length == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < count; ++i) {
        size_t current_length = list_length(lists[i]);

        if (longest == NULL || current_length > best_length) {
            longest = lists[i];
            best_index = i;
            best_length = current_length;
        }
    }

    *longest_index = best_index;
    *longest_length = best_length;
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
    const int values1[] = {1, 2, 3};
    const int values2[] = {4, 5, 6, 7, 8};
    const int values3[] = {9, 10, 11, 12};
    Node *lists[3] = {NULL, NULL, NULL};
    size_t longest_index = 0;
    size_t longest_length = 0;
    const Node *longest = NULL;

    lists[0] = create_list(values1, sizeof(values1) / sizeof(values1[0]));
    lists[1] = create_list(values2, sizeof(values2) / sizeof(values2[0]));
    lists[2] = create_list(values3, sizeof(values3) / sizeof(values3[0]));

    if (lists[0] == NULL || lists[1] == NULL || lists[2] == NULL) {
        for (size_t i = 0; i < sizeof(lists) / sizeof(lists[0]); ++i) {
            free_list(lists[i]);
        }
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    longest = find_longest_list(
        (const Node *const *)lists,
        sizeof(lists) / sizeof(lists[0]),
        &longest_index,
        &longest_length
    );

    if (longest == NULL) {
        for (size_t i = 0; i < sizeof(lists) / sizeof(lists[0]); ++i) {
            free_list(lists[i]);
        }
        fprintf(stderr, "Unable to find the longest list\n");
        return EXIT_FAILURE;
    }

    printf("Longest list index: %zu\n", longest_index);
    printf("Longest list length: %zu\n", longest_length);
    print_list(longest);

    for (size_t i = 0; i < sizeof(lists) / sizeof(lists[0]); ++i) {
        free_list(lists[i]);
    }

    return EXIT_SUCCESS;
}