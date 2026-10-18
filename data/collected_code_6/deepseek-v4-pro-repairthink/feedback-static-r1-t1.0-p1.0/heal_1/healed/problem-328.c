#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node* next;
};

struct Node* createNode(int data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    if (newNode == NULL) {
        return NULL;
    }
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

struct Node* insertEnd(struct Node* head, int data) {
    struct Node* newNode = createNode(data);
    if (newNode == NULL) {
        return head;
    }
    if (head == NULL) {
        return newNode;
    }
    struct Node* temp = head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = newNode;
    return head;
}

struct Node* rotateLeft(struct Node* head, int k) {
    if (head == NULL || head->next == NULL || k <= 0) {
        return head;
    }
    
    struct Node* current = head;
    int length = 1;
    while (current->next != NULL) {
        current = current->next;
        length++;
    }
    
    k = k % length;
    if (k == 0) {
        return head;
    }
    
    current->next = head;
    
    current = head;
    for (int i = 1; i < k; i++) {
        current = current->next;
    }
    
    head = current->next;
    current->next = NULL;
    
    return head;
}

void printList(struct Node* head) {
    struct Node* temp = head;
    while (temp != NULL) {
        printf("%d ", temp->data);
        temp = temp->next;
    }
    printf("\n");
}

void freeList(struct Node* head) {
    while (head != NULL) {
        struct Node* temp = head;
        head = head->next;
        free(temp);
    }
}

int main() {
    struct Node* head = NULL;
    
    head = insertEnd(head, 1);
    head = insertEnd(head, 2);
    head = insertEnd(head, 3);
    head = insertEnd(head, 4);
    head = insertEnd(head, 5);
    
    if (head == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    printf("Original list: ");
    printList(head);
    
    head = rotateLeft(head, 2);
    
    printf("Rotated list: ");
    printList(head);
    
    freeList(head);
    
    return 0;
}