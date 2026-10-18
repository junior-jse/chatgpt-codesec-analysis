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

void append_node(Node **head, int data)
{
    Node *new_node = create_node(data);
    if (new_node == NULL) {
        return;
    }
    if (*head == NULL) {
        *head = new_node;
        return;
    }
    Node *current = *head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = new_node;
}

int is_in_list(Node *list, int value)
{
    while (list != NULL) {
        if (list->data == value) {
            return 1;
        }
        list = list->next;
    }
    return 0;
}

void remove_elements(Node **head, Node *remove_list)
{
    if (head == NULL || *head == NULL || remove_list == NULL) {
        return;
    }
    Node *current = *head;
    Node *prev = NULL;
    while (current != NULL) {
        if (is_in_list(remove_list, current->data)) {
            Node *temp = current;
            if (prev == NULL) {
                *head = current->next;
            } else {
                prev->next = current->next;
            }
            current = current->next;
            free(temp);
        } else {
            prev = current;
            current = current->next;
        }
    }
}

void free_list(Node **head)
{
    if (head == NULL) {
        return;
    }
    Node *current = *head;
    while (current != NULL) {
        Node *temp = current;
        current = current->next;
        free(temp);
    }
    *head = NULL;
}

void print_list(Node *head)
{
    while (head != NULL) {
        printf("%d ", head->data);
        head = head->next;
    }
    printf("\n");
}

int main(void)
{
    Node *list1 = NULL;
    Node *list2 = NULL;

    append_node(&list1, 1);
    append_node(&list1, 2);
    append_node(&list1, 3);
    append_node(&list1, 4);
    append_node(&list1, 5);

    append_node(&list2, 2);
    append_node(&list2, 4);

    printf("Original list: ");
    print_list(list1);

    remove_elements(&list1, list2);

    printf("After removal: ");
    print_list(list1);

    free_list(&list1);
    free_list(&list2);

    return 0;
}