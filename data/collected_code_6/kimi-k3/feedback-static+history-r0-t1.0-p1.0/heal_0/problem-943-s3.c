#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

Node *create_node(int data)
{
    Node *new_node = malloc(sizeof(Node));
    if (new_node == NULL) {
        return NULL;
    }
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}

Node *merge_sorted_lists(Node *list1, Node *list2)
{
    Node dummy;
    Node *tail = &dummy;
    dummy.next = NULL;

    while (list1 != NULL && list2 != NULL) {
        if (list1->data <= list2->data) {
            tail->next = list1;
            list1 = list1->next;
        } else {
            tail->next = list2;
            list2 = list2->next;
        }
        tail = tail->next;
    }

    if (list1 != NULL) {
        tail->next = list1;
    } else {
        tail->next = list2;
    }

    return dummy.next;
}

Node *append_node(Node *head, int data)
{
    Node *new_node = create_node(data);
    if (new_node == NULL) {
        return NULL;
    }

    if (head == NULL) {
        return new_node;
    }

    Node *current = head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = new_node;
    return head;
}

void print_list(const Node *head)
{
    const Node *current = head;
    while (current != NULL) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");
}

void free_list(Node *head)
{
    Node *current = head;
    while (current != NULL) {
        Node *temp = current;
        current = current->next;
        free(temp);
    }
}

int main(void)
{
    Node *list1 = NULL;
    Node *list2 = NULL;

    list1 = append_node(list1, 1);
    if (list1 == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }
    if (append_node(list1, 3) == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free_list(list1);
        return EXIT_FAILURE;
    }
    if (append_node(list1, 5) == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free_list(list1);
        return EXIT_FAILURE;
    }

    list2 = append_node(list2, 2);
    if (list2 == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free_list(list1);
        return EXIT_FAILURE;
    }
    if (append_node(list2, 4) == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free_list(list1);
        free_list(list2);
        return EXIT_FAILURE;
    }
    if (append_node(list2, 6) == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free_list(list1);
        free_list(list2);
        return EXIT_FAILURE;
    }

    printf("List 1: ");
    print_list(list1);
    printf("List 2: ");
    print_list(list2);

    Node *merged = merge_sorted_lists(list1, list2);

    printf("Merged: ");
    print_list(merged);

    free_list(merged);

    return EXIT_SUCCESS;
}