#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node* next;
} Node;

Node* createNode(int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

Node* removeKthElement(Node* head, int k) {
    if (head == NULL) {
        return NULL;
    }
    
    if (k <= 0) {
        return head;
    }
    
    if (k == 1) {
        Node* temp = head;
        head = head->next;
        free(temp);
        return head;
    }
    
    Node* current = head;
    Node* previous = NULL;
    int count = 1;
    
    while (current != NULL && count < k) {
        previous = current;
        current = current->next;
        count++;
    }
    
    if (current == NULL) {
        return head;
    }
    
    previous->next = current->next;
    free(current);
    
    return head;
}

void printList(Node* head) {
    Node* current = head;
    while (current != NULL) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");
}

void freeList(Node* head) {
    Node* current = head;
    while (current != NULL) {
        Node* temp = current;
        current = current->next;
        free(temp);
    }
}

int main() {
    Node* head = createNode(1);
    head->next = createNode(2);
    head->next->next = createNode(3);
    head->next->next->next = createNode(4);
    head->next->next->next->next = createNode(5);
    
    printf("Original list: ");
    printList(head);
    
    head = removeKthElement(head, 3);
    
    printf("After removing 3rd element: ");
    printList(head);
    
    head = removeKthElement(head, 1);
    
    printf("After removing 1st element: ");
    printList(head);
    
    head = removeKthElement(head, 10);
    
    printf("After attempting to remove 10th element: ");
    printList(head);
    
    freeList(head);
    
    return 0;
}