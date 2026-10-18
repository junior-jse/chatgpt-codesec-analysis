#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

Node *create_node(int value)
{
    Node *new_node = malloc(sizeof(Node));
    if (new_node == NULL) {
        return NULL;
    }
    new_node->data = value;
    new_node->next = NULL;
    return new_node;
}

void free_list(Node *head)
{
    Node *current = head;
    Node *next;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
}

Node *merge_sorted_lists(Node *list1, Node *list2)
{
    if (list1 == NULL) {
        return list2;
    }
    if (list2 == NULL) {
        return list1;
    }

    Node *merged_head = NULL;
    Node *merged_tail = NULL;

    while (list1 != NULL && list2 != NULL) {
        Node *selected;
        if (list1->data <= list2->data) {
            selected = list1;
            list1 = list1->next;
        } else {
            selected = list2;
            list2 = list2->next;
        }

        if (merged_head == NULL) {
            merged_head = selected;
            merged_tail = selected;
        } else {
            merged_tail->next = selected;
            merged_tail = selected;
        }
    }

    if (list1 != NULL) {
        merged_tail->next = list1;
    } else {
        merged_tail->next = list2;
    }

    return merged_head;
}

int append_node(Node **head, Node **tail, int value)
{
    Node *new_node = create_node(value);
    if (new_node == NULL) {
        return -1;
    }

    if (*head == NULL) {
        *head = new_node;
        *tail = new_node;
    } else {
        (*tail)->next = new_node;
        *tail = new_node;
    }
    return 0;
}

void print_list(const Node *head)
{
    const Node *current = head;
    while (current != NULL) {
        printf("%d", current->data);
        if (current->next != NULL) {
            printf(" -> ");
        }
        current = current->next;
    }
    printf("\n");
}

int main(void)
{
    Node *list1_head = NULL;
    Node *list1_tail = NULL;
    Node *list2_head = NULL;
    Node *list2_tail = NULL;
    Node *merged = NULL;

    if (append_node(&list1_head, &list1_tail, 1) != 0 ||
        append_node(&list1_head, &list1_tail, 3) != 0 ||
        append_node(&list1_head, &list1_tail, 5) != 0 ||
        append_node(&list1_head, &list1_tail, 7) != 0) {
        free_list(list1_head);
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    if (append_node(&list2_head, &list2_tail, 2) != 0 ||
        append_node(&list2_head, &list2_tail, 4) != 0 ||
        append_node(&list2_head, &list2_tail, 6) != 0 ||
        append_node(&list2_head, &list2_tail, 8) != 0) {
        free_list(list1_head);
        free_list(list2_head);
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    printf("List 1: ");
    print_list(list1_head);
    printf("List 2: ");
    print_list(list2_head);

    merged = merge_sorted_lists(list1_head, list2_head);

    printf("Merged: ");
    print_list(merged);

    free_list(merged);
    return EXIT_SUCCESS;
}