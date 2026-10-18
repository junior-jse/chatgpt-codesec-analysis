#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

Node* create_node(int data) {
    Node *new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL) {
        return NULL;
    }
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}

void append(Node **head, int data) {
    Node *new_node = create_node(data);
    if (new_node == NULL) {
        return;
    }
    if (*head == NULL) {
        *head = new_node;
        return;
    }
    Node *temp = *head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = new_node;
}

int contains(Node *head, int value) {
    Node *temp = head;
    while (temp != NULL) {
        if (temp->data == value) {
            return 1;
        }
        temp = temp->next;
    }
    return 0;
}

Node* subtract_lists(Node *list1, Node *list2) {
    Node *result = NULL;
    Node *temp = list1;
    while (temp != NULL) {
        if (!contains(list2, temp->data)) {
            append(&result, temp->data);
        }
        temp = temp->next;
    }
    return result;
}

void print_list(Node *head) {
    Node *temp = head;
    while (temp != NULL) {
        printf("%d ", temp->data);
        temp = temp->next;
    }
    printf("\n");
}

void free_list(Node *head) {
    while (head != NULL) {
        Node *temp = head;
        head = head->next;
        free(temp);
    }
}

int main(void) {
    Node *list1 = NULL;
    Node *list2 = NULL;
    Node *result = NULL;

    append(&list1, 1);
    append(&list1, 2);
    append(&list1, 3);
    append(&list1, 4);
    append(&list1, 5);

    append(&list2, 2);
    append(&list2, 4);

    result = subtract_lists(list1, list2);

    printf("List 1: ");
    print_list(list1);
    printf("List 2: ");
    print_list(list2);
    printf("Result: ");
    print_list(result);

    free_list(list1);
    free_list(list2);
    free_list(result);

    return 0;
}