#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

Node *createNode(int data) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

void appendNode(Node **head, int data) {
    Node *newNode = createNode(data);
    if (*head == NULL) {
        *head = newNode;
        return;
    }
    Node *current = *head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = newNode;
}

void freeList(Node *head) {
    Node *current = head;
    while (current != NULL) {
        Node *next = current->next;
        free(current);
        current = next;
    }
}

void printList(Node *head) {
    Node *current = head;
    while (current != NULL) {
        printf("%d", current->data);
        if (current->next != NULL) {
            printf(" -> ");
        }
        current = current->next;
    }
    printf("\n");
}

Node *subtractLists(Node *list1, Node *list2) {
    Node *result = NULL;
    Node *current1 = list1;
    Node *current2 = list2;

    while (current1 != NULL || current2 != NULL) {
        int value1 = (current1 != NULL) ? current1->data : 0;
        int value2 = (current2 != NULL) ? current2->data : 0;
        appendNode(&result, value1 - value2);
        
        if (current1 != NULL) current1 = current1->next;
        if (current2 != NULL) current2 = current2->next;
    }

    return result;
}

int main(void) {
    Node *list1 = NULL;
    Node *list2 = NULL;
    Node *result = NULL;

    appendNode(&list1, 10);
    appendNode(&list1, 20);
    appendNode(&list1, 30);
    appendNode(&list1, 40);

    appendNode(&list2, 5);
    appendNode(&list2, 15);
    appendNode(&list2, 25);

    printf("List 1: ");
    printList(list1);
    printf("List 2: ");
    printList(list2);

    result = subtractLists(list1, list2);
    printf("Result: ");
    printList(result);

    freeList(list1);
    freeList(list2);
    freeList(result);

    return 0;
}