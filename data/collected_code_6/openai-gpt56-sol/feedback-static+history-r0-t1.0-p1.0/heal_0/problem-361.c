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

static int append_value(IntNode **head, int value)
{
    IntNode *node;
    IntNode **current;

    if (head == NULL) {
        return 0;
    }

    node = create_int_node(value);
    if (node == NULL) {
        return 0;
    }

    current = head;
    while (*current != NULL) {
        current = &(*current)->next;
    }
    *current = node;
    return 1;
}

static int append_list(ListNode **head, IntNode *list)
{
    ListNode *node;
    ListNode **current;

    if (head == NULL) {
        return 0;
    }

    node = create_list_node(list);
    if (node == NULL) {
        return 0;
    }

    current = head;
    while (*current != NULL) {
        current = &(*current)->next;
    }
    *current = node;
    return 1;
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

static void print_list_of_lists(const ListNode *head)
{
    putchar('[');

    while (head != NULL) {
        const IntNode *item = head->list;

        putchar('[');
        while (item != NULL) {
            printf("%d", item->value);
            if (item->next != NULL) {
                printf(", ");
            }
            item = item->next;
        }
        putchar(']');

        if (head->next != NULL) {
            printf(", ");
        }
        head = head->next;
    }

    puts("]");
}

int main(void)
{
    ListNode *lists = NULL;
    IntNode *first = NULL;
    IntNode *third = NULL;

    if (!append_value(&first, 1) ||
        !append_value(&first, 2) ||
        !append_value(&third, 3)) {
        free_int_list(first);
        free_int_list(third);
        return EXIT_FAILURE;
    }

    if (!append_list(&lists, first)) {
        free_int_list(first);
        free_int_list(third);
        return EXIT_FAILURE;
    }
    first = NULL;

    if (!append_list(&lists, NULL)) {
        free_int_list(third);
        free_list_of_lists(lists);
        return EXIT_FAILURE;
    }

    if (!append_list(&lists, third)) {
        free_int_list(third);
        free_list_of_lists(lists);
        return EXIT_FAILURE;
    }
    third = NULL;

    if (!append_list(&lists, NULL)) {
        free_list_of_lists(lists);
        return EXIT_FAILURE;
    }

    remove_empty_lists(&lists);
    print_list_of_lists(lists);
    free_list_of_lists(lists);

    return EXIT_SUCCESS;
}