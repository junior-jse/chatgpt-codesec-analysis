#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node* next;
} Node;

Node* create_node(int data) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL) {
        return NULL;
    }
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}

void append_node(Node** head, int data) {
    Node* new_node = create_node(data);
    if (new_node == NULL) {
        return;
    }
    
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

Node* add_lists(Node* list1, Node* list2) {
    if (list1 == NULL || list2 == NULL) {
        return NULL;
    }
    
    Node* result = NULL;
    Node* current1 = list1;
    Node* current2 = list2;
    
    while (current1 != NULL && current2 != NULL) {
        int sum = current1->data + current2->data;
        append_node(&result, sum);
        current1 = current1->next;
        current2 = current2->next;
    }
    
    while (current1 != NULL) {
        append_node(&result, current1->data);
        current1 = current1->next;
    }
    
    while (current2 != NULL) {
        append_node(&result, current2->data);
        current2 = current2->next;
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

void free_list(Node* head) {
    Node* current = head;
    while (current != NULL) {
        Node* temp = current;
        current = current->next;
        free(temp);
    }
}

int main(void) {
    Node* list1 = NULL;
    Node* list2 = NULL;
    
    append_node(&list1, 1);
    append_node(&list1, 2);
    append_node(&list1, 3);
    append_node(&list1, 4);
    
    append_node(&list2, 5);
    append_node(&list2, 6);
    append_node(&list2, 7);
    
    printf("List 1: ");
    print_list(list1);
    printf("List 2: ");
    print_list(list2);
    
    Node* result = add_lists(list1, list2);
    
    if (result != NULL) {
        printf("Result: ");
        print_list(result);
    }
    
    free_list(list1);
    free_list(list2);
    free_list(result);
    
    return 0;
}