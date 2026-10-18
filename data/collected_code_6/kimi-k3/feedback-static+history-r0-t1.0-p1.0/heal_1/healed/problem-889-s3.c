#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node* next;
} Node;

typedef struct List {
    Node* head;
} List;

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

void appendNode(List* list, int data) {
    Node* newNode = createNode(data);
    if (list->head == NULL) {
        list->head = newNode;
    } else {
        Node* temp = list->head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newNode;
    }
}

void reverseList(List* list) {
    Node* prev = NULL;
    Node* current = list->head;
    Node* next = NULL;
    while (current != NULL) {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    list->head = prev;
}

void printList(const List* list) {
    Node* temp = list->head;
    while (temp != NULL) {
        printf("%d -> ", temp->data);
        temp = temp->next;
    }
    printf("NULL\n");
}

void freeList(List* list) {
    while (list->head != NULL) {
        Node* temp = list->head;
        list->head = list->head->next;
        free(temp);
    }
}

int main() {
    List list1 = {NULL};
    List list2 = {NULL};
    List list3 = {NULL};

    appendNode(&list1, 1);
    appendNode(&list1, 2);
    appendNode(&list1, 3);

    appendNode(&list2, 4);
    appendNode(&list2, 5);
    appendNode(&list2, 6);

    appendNode(&list3, 7);
    appendNode(&list3, 8);
    appendNode(&list3, 9);

    List* lists[] = {&list1, &list2, &list3};
    int numLists = sizeof(lists) / sizeof(lists[0]);

    for (int i = 0; i < numLists; i++) {
        printf("Original List %d: ", i + 1);
        printList(lists[i]);
        reverseList(lists[i]);
        printf("Reversed List %d: ", i + 1);
        printList(lists[i]);
        freeList(lists[i]);
    }

    return 0;
}