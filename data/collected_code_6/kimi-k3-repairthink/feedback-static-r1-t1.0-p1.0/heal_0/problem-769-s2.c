#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node* next;
} Node;

Node* createNode(int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        return NULL;
    }
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

void insertNode(Node** head, int data) {
    Node* newNode = createNode(data);
    if (newNode == NULL) {
        return;
    }
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

int isInList(Node* head, int data) {
    Node* current = head;
    while (current != NULL) {
        if (current->data == data) {
            return 1;
        }
        current = current->next;
    }
    return 0;
}

Node* getDifference(Node* list1, Node* list2) {
    Node* result = NULL;
    Node* current = list1;
    
    while (current != NULL) {
        if (!isInList(list2, current->data)) {
            insertNode(&result, current->data);
        }
        current = current->next;
    }
    
    return result;
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
    Node* next;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
}

int main() {
    Node* list1 = NULL;
    Node* list2 = NULL;
    Node* difference = NULL;
    
    insertNode(&list1, 1);
    insertNode(&list1, 2);
    insertNode(&list1, 3);
    insertNode(&list1, 4);
    insertNode(&list1, 5);
    
    insertNode(&list2, 2);
    insertNode(&list2, 4);
    insertNode(&list2, 6);
    
    printf("List 1: ");
    printList(list1);
    
    printf("List 2: ");
    printList(list2);
    
    difference = getDifference(list1, list2);
    
    printf("Difference: ");
    printList(difference);
    
    freeList(list1);
    freeList(list2);
    freeList(difference);
    
    return 0;
}