#include <stdio.h>
#include <stdlib.h>

typedef struct IntNode {
    int value;
    struct IntNode *next;
} IntNode;

typedef struct ListNode {
    IntNode *list;
    struct ListNode *next;
} ListNode;

static void free_int_list(IntNode *head)
{
    while (head != NULL) {
        IntNode *next = head->next;
        free(head);
        head = next;
    }
}

static void free_list_of_lists(ListNode *head)
{
    while (head != NULL) {
        ListNode *next = head->next;
        free_int_list(head->list);
        free(head);
        head = next;
    }
}

static IntNode *create_int_node(int value)
{
    IntNode *node = malloc(sizeof(*node));

    if (node == NULL) {
        return NULL;
    }

    node->value = value;
    node->next = NULL;
    return node;
}

static ListNode *create_list_node(IntNode *list)
{
    ListNode *node = malloc(sizeof(*node));

    if (node == NULL) {
        return NULL;
    }

    node->list = list;
    node->next = NULL;
    return node;
}

static void remove_empty_lists(ListNode **head)
{
    ListNode **current;

    if (head == NULL) {
        return;
    }

    current = head;

    while (*current != NULL) {
        if ((*current)->list == NULL) {
            ListNode *empty = *current;
            *current = empty->next;
            free(empty);
        } else {
            current = &(*current)->next;
        }
    }
}

static void print_list_of_lists(const ListNode *head)
{
    while (head != NULL) {
        const IntNode *item = head->list;

        putchar('[');

        while (item != NULL) {
            printf("%d", item->value);
            item = item->next;

            if (item != NULL) {
                printf(", ");
            }
        }

        putchar(']');
        head = head->next;

        if (head != NULL) {
            printf(" -> ");
        }
    }

    putchar('\n');
}

int main(void)
{
    ListNode *first;
    ListNode *second;
    ListNode *third;
    ListNode *fourth;
    IntNode *one;
    IntNode *two;
    IntNode *three;

    one = create_int_node(1);
    two = create_int_node(2);
    three = create_int_node(3);

    if (one == NULL || two == NULL || three == NULL) {
        free(one);
        free(two);
        free(three);
        return EXIT_FAILURE;
    }

    one->next = two;

    first = create_list_node(NULL);
    second = create_list_node(one);
    third = create_list_node(NULL);
    fourth = create_list_node(three);

    if (first == NULL || second == NULL || third == NULL || fourth == NULL) {
        free(first);
        free(second);
        free(third);
        free(fourth);
        free_int_list(one);
        free_int_list(three);
        return EXIT_FAILURE;
    }

    first->next = second;
    second->next = third;
    third->next = fourth;

    remove_empty_lists(&first);
    print_list_of_lists(first);
    free_list_of_lists(first);

    return EXIT_SUCCESS;
}