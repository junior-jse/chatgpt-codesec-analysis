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
        exit(EXIT_FAILURE);
    }
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

void appendNode(Node** head, int data) {
    Node* newNode = createNode(data);
    if (*head == NULL) {
        *head = newNode;
        return;
    }
    Node* current = *head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = newNode;
}

void interchangeFirstAndLast(Node** head) {
    if (*head == NULL || (*head)->next == NULL) {
        return;
    }
    
    Node* first = *head;
    Node* secondLast = NULL;
    Node* last = *head;
    
    while (last->next != NULL) {
        secondLast = last;
        last = last->next;
    }
    
    if (first->next == last) {
        last->next = first;
        first->next = NULL;
        *head = last;
        return;
    }
    
    last->next = first->next;
    secondLast->next = first;
    first->next = NULL;
    *head = last;
}

void freeList(Node* head) {
    Node* current = head;
    while (current != NULL) {
        Node* temp = current;
        current = current->next;
        free(temp);
    }
}

void printList(Node* head) {
    Node* current = head;
    while (current != NULL) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");
}

int main(void) {
    Node* head = NULL;
    
    appendNode(&head, 10);
    appendNode(&head, 20);
    appendNode(&head, 30);
    appendNode(&head, 40);
    appendNode(&head, 50);
    
    printf("Original list: ");
    printList(head);
    
    interchangeFirstAndLast(&head);
    
    printf("After interchange: ");
    printList(head);
    
    freeList(head);
    
    return 0;
}