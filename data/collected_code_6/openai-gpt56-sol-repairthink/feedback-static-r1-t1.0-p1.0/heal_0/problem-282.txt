#include <stdio.h>
#include <stdlib.h>

typedef struct ListNode {
    int value;
    struct ListNode *next;
} ListNode;

static void free_list(ListNode *head)
{
    while (head != NULL) {
        ListNode *next = head->next;
        free(head);
        head = next;
    }
}

static int append_node(ListNode **head, ListNode **tail, int value)
{
    ListNode *node = malloc(sizeof(*node));

    if (node == NULL) {
        return 0;
    }

    node->value = value;
    node->next = NULL;

    if (*tail == NULL) {
        *head = node;
    } else {
        (*tail)->next = node;
    }

    *tail = node;
    return 1;
}

static ListNode *subtract_lists(const ListNode *first, const ListNode *second)
{
    ListNode *result = NULL;
    ListNode *tail = NULL;

    while (first != NULL && second != NULL) {
        if (!append_node(&result, &tail, first->value - second->value)) {
            free_list(result);
            return NULL;
        }

        first = first->next;
        second = second->next;
    }

    while (first != NULL) {
        if (!append_node(&result, &tail, first->value)) {
            free_list(result);
            return NULL;
        }

        first = first->next;
    }

    while (second != NULL) {
        if (!append_node(&result, &tail, -second->value)) {
            free_list(result);
            return NULL;
        }

        second = second->next;
    }

    return result;
}

static void print_list(const ListNode *head)
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
    const int first_values[] = {10, 20, 30, 40};
    const int second_values[] = {1, 2, 3};
    const size_t first_count = sizeof(first_values) / sizeof(first_values[0]);
    const size_t second_count = sizeof(second_values) / sizeof(second_values[0]);
    ListNode *first = NULL;
    ListNode *first_tail = NULL;
    ListNode *second = NULL;
    ListNode *second_tail = NULL;
    ListNode *result = NULL;

    for (size_t i = 0; i < first_count; ++i) {
        if (!append_node(&first, &first_tail, first_values[i])) {
            fprintf(stderr, "Failed to allocate memory\n");
            free_list(first);
            return EXIT_FAILURE;
        }
    }

    for (size_t i = 0; i < second_count; ++i) {
        if (!append_node(&second, &second_tail, second_values[i])) {
            fprintf(stderr, "Failed to allocate memory\n");
            free_list(first);
            free_list(second);
            return EXIT_FAILURE;
        }
    }

    result = subtract_lists(first, second);
    if (result == NULL && (first != NULL || second != NULL)) {
        fprintf(stderr, "Failed to allocate memory\n");
        free_list(first);
        free_list(second);
        return EXIT_FAILURE;
    }

    print_list(result);

    free_list(first);
    free_list(second);
    free_list(result);

    return EXIT_SUCCESS;
}