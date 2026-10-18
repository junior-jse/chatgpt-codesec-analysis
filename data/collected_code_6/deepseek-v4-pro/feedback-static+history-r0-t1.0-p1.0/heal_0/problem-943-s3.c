#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

Node *createNode(int data) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL) {
        exit(EXIT_FAILURE);
    }
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

Node *insertEnd(Node *head, int data) {
    Node *newNode = createNode(data);
    if (head == NULL) {
        return newNode;
    }
    Node *current = head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = newNode;
    return head;
}

Node *mergeSortedLists(Node *list1, Node *list2) {
    if (list1 == NULL) {
        return list2;
    }
    if (list2 == NULL) {
        return list1;
    }
    
    Node *result = NULL;
    Node *tail = NULL;
    
    while (list1 != NULL && list2 != NULL) {
        Node *selectedNode = NULL;
        if (list1->data <= list2->data) {
            selectedNode = list1;
            list1 = list1->next;
        } else {
            selectedNode = list2;
            list2 = list2->next;
        }
        
        selectedNode->next = NULL;
        if (result == NULL) {
            result = selectedNode;
            tail = selectedNode;
        } else {
            tail->next = selectedNode;
            tail = tail->next;
        }
    }
    
    if (list1 != NULL) {
        tail->next = list1;
    }
    if (list2 != NULL) {
        tail->next = list2;
    }
    
    return result;
}

void printList(Node *head) {
    Node *current = head;
    while (current != NULL) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");
}

void freeList(Node *head) {
    Node *current = head;
    Node *nextNode = NULL;
    while (current != NULL) {
        nextNode = current->next;
        free(current);
        current = nextNode;
    }
}

int main(void) {
    Node *list1 = NULL;
    Node *list2 = NULL;
    Node *mergedList = NULL;
    
    list1 = insertEnd(list1, 1);
    list1 = insertEnd(list1, 3);
    list1 = insertEnd(list1, 5);
    list1 = insertEnd(list1, 7);
    
    list2 = insertEnd(list2, 2);
    list2 = insertEnd(list2, 4);
    list2 = insertEnd(list2, 6);
    list2 = insertEnd(list2, 8);
    
    printf("List 1: ");
    printList(list1);
    printf("List 2: ");
    printList(list2);
    
    mergedList = mergeSortedLists(list1, list2);
    
    printf("Merged list: ");
    printList(mergedList);
    
    freeList(mergedList);
    
    return 0;
}