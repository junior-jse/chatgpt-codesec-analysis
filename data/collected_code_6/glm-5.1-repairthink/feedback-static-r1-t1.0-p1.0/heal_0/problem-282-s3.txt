#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

Node *create_node(int data) {
    Node *node = (Node *)malloc(sizeof(Node));
    if (node == NULL) {
        exit(EXIT_FAILURE);
    }
    node->data = data;
    node->next = NULL;
    return node;
}

void free_list(Node *head) {
    Node *current = head;
    Node *next;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
}

Node *subtract_lists(Node *a, Node *b) {
    Node *result = NULL;
    Node **tail = &result;

    while (a != NULL) {
        int val = a->data;
        if (b != NULL) {
            val -= b->data;
            b = b->next;
        }
        Node *new_node = create_node(val);
        *tail = new_node;
        tail = &(new_node->next);
        a = a->next;
    }

    return result;
}

Node *append(Node *head, int data) {
    Node *new_node = create_node(data);
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

void print_list(Node *head) {
    Node *current = head;
    while (current != NULL) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");
}

int main(void) {
    Node *list1 = NULL;
    list1 = append(list1, 10);
    list1 = append(list1, 20);
    list1 = append(list1, 30);

    Node *list2 = NULL;
    list2 = append(list2, 1);
    list2 = append(list2, 2);

    Node *diff = subtract_lists(list1, list2);

    print_list(diff);

    free_list(list1);
    free_list(list2);
    free_list(diff);

    return 0;
}