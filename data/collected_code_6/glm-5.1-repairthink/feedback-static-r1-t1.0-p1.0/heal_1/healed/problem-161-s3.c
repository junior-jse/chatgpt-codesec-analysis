#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node* next;
} Node;

Node* create_node(int data) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL) {
        exit(EXIT_FAILURE);
    }
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}

void append_node(Node** head_ref, int data) {
    Node* new_node = create_node(data);
    if (*head_ref == NULL) {
        *head_ref = new_node;
        return;
    }
    Node* last = *head_ref;
    while (last->next != NULL) {
        last = last->next;
    }
    last->next = new_node;
}

void free_list(Node* head) {
    while (head != NULL) {
        Node* tmp = head;
        head = head->next;
        free(tmp);
    }
}

int is_present(Node* head, int data) {
    Node* current = head;
    while (current != NULL) {
        if (current->data == data) {
            return 1;
        }
        current = current->next;
    }
    return 0;
}

Node* remove_elements(Node* list1, Node* list2) {
    Node* result = NULL;
    Node** tail = &result;
    Node* current = list1;

    while (current != NULL) {
        if (!is_present(list2, current->data)) {
            Node* new_node = create_node(current->data);
            *tail = new_node;
            tail = &((*tail)->next);
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

int main() {
    Node* list1 = NULL;
    Node* list2 = NULL;
    Node* result = NULL;

    append_node(&list1, 1);
    append_node(&list1, 2);
    append_node(&list1, 3);
    append_node(&list1, 4);
    append_node(&list1, 5);

    append_node(&list2, 2);
    append_node(&list2, 4);
    append_node(&list2, 6);

    result = remove_elements(list1, list2);
    print_list(result);

    free_list(list1);
    free_list(list2);
    free_list(result);

    return 0;
}