#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node* next;
};

struct Node* createNode(int data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    if (newNode == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

void appendNode(struct Node** headRef, int data) {
    struct Node* newNode = createNode(data);
    if (*headRef == NULL) {
        *headRef = newNode;
        return;
    }
    struct Node* current = *headRef;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = newNode;
}

void freeList(struct Node* head) {
    struct Node* current = head;
    while (current != NULL) {
        struct Node* next = current->next;
        free(current);
        current = next;
    }
}

int getLength(struct Node* head) {
    int length = 0;
    struct Node* current = head;
    while (current != NULL) {
        length++;
        current = current->next;
    }
    return length;
}

struct Node* rotateRight(struct Node* head, int k) {
    if (head == NULL || head->next == NULL || k == 0) {
        return head;
    }
    
    int length = getLength(head);
    k = k % length;
    
    if (k == 0) {
        return head;
    }
    
    struct Node* current = head;
    int stepsToNewHead = length - k - 1;
    
    for (int i = 0; i < stepsToNewHead; i++) {
        current = current->next;
    }
    
    struct Node* newHead = current->next;
    current->next = NULL;
    
    struct Node* tail = newHead;
    while (tail->next != NULL) {
        tail = tail->next;
    }
    tail->next = head;
    
    return newHead;
}

void printList(struct Node* head) {
    struct Node* current = head;
    while (current != NULL) {
        printf("%d", current->data);
        if (current->next != NULL) {
            printf(" -> ");
        }
        current = current->next;
    }
    printf("\n");
}

int main() {
    struct Node* head = NULL;
    int values[] = {1, 2, 3, 4, 5};
    int size = sizeof(values) / sizeof(values[0]);
    
    for (int i = 0; i < size; i++) {
        appendNode(&head, values[i]);
    }
    
    printf("Original list: ");
    printList(head);
    
    int rotateBy = 2;
    head = rotateRight(head, rotateBy);
    
    printf("After rotating by %d: ", rotateBy);
    printList(head);
    
    freeList(head);
    
    return 0;
}