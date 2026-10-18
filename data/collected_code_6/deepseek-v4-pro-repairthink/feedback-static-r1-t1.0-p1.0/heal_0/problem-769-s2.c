#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node* next;
} Node;

Node* create_node(int data) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}

void append_node(Node** head, int data) {
    Node* new_node = create_node(data);
    if (*head == NULL) {
        *head = new_node;
        return;
    }
    Node* current = *head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = new_node;
}

void free_list(Node* head) {
    Node* current = head;
    while (current != NULL) {
        Node* next = current->next;
        free(current);
        current = next;
    }
}

int contains(Node* head, int value) {
    Node* current = head;
    while (current != NULL) {
        if (current->data == value) {
            return 1;
        }
        current = current->next;
    }
    return 0;
}

Node* list_difference(Node* list1, Node* list2) {
    Node* result = NULL;
    Node* current = list1;
    while (current != NULL) {
        if (!contains(list2, current->data)) {
            append_node(&result, current->data);
        }
        current = current->next;
    }
    return result;
}

void print_list(Node* head) {
    Node* current = head;
    while (current != NULL) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");
}

int main(void) {
    Node* list1 = NULL;
    Node* list2 = NULL;
    Node* difference = NULL;

    append_node(&list1, 1);
    append_node(&list1, 2);
    append_node(&list1, 3);
    append_node(&list1, 4);
    append_node(&list1, 5);

    append_node(&list2, 2);
    append_node(&list2, 4);
    append_node(&list2, 6);

    difference = list_difference(list1, list2);

    printf("List 1: ");
    print_list(list1);
    printf("List 2: ");
    print_list(list2);
    printf("Difference (List1 - List2): ");
    print_list(difference);

    free_list(list1);
    free_list(list2);
    free_list(difference);

    return 0;
}