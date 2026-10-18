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

void append_node(Node **head, Node **tail, int data)
{
    Node *new_node = create_node(data);
    if (new_node == NULL) {
        return;
    }
    if (*head == NULL) {
        *head = new_node;
        *tail = new_node;
    } else {
        (*tail)->next = new_node;
        *tail = new_node;
    }
}

Node *interleave_lists(Node *list1, Node *list2)
{
    Node *result_head = NULL;
    Node *result_tail = NULL;

    while (list1 != NULL && list2 != NULL) {
        append_node(&result_head, &result_tail, list1->data);
        append_node(&result_head, &result_tail, list2->data);
        list1 = list1->next;
        list2 = list2->next;
    }

    return result_head;
}

void print_list(Node *head)
{
    Node *current = head;
    while (current != NULL) {
        printf("%d", current->data);
        if (current->next != NULL) {
            printf(" -> ");
        }
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
    Node *list1_tail = NULL;
    Node *list2 = NULL;
    Node *list2_tail = NULL;

    append_node(&list1, &list1_tail, 1);
    append_node(&list1, &list1_tail, 3);
    append_node(&list1, &list1_tail, 5);

    append_node(&list2, &list2_tail, 2);
    append_node(&list2, &list2_tail, 4);
    append_node(&list2, &list2_tail, 6);

    printf("List 1: ");
    print_list(list1);

    printf("List 2: ");
    print_list(list2);

    Node *interleaved = interleave_lists(list1, list2);

    printf("Interleaved: ");
    print_list(interleaved);

    free_list(list1);
    free_list(list2);
    free_list(interleaved);

    return 0;
}