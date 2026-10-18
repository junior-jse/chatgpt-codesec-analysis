#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node *next;
};

struct Node *createNode(int data) {
    struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
    if (newNode == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

void insertBeforeEach(struct Node **head, int value) {
    if (head == NULL || *head == NULL) {
        return;
    }

    struct Node *current = *head;
    struct Node *prev = NULL;

    while (current != NULL) {
        struct Node *newNode = createNode(value);
        newNode->next = current;

        if (prev == NULL) {
            *head = newNode;
        } else {
            prev->next = newNode;
        }

        prev = current;
        current = current->next;
    }
}

void freeList(struct Node *head) {
    struct Node *current = head;
    while (current != NULL) {
        struct Node *temp = current;
        current = current->next;
        free(temp);
    }
}

void printList(struct Node *head) {
    struct Node *current = head;
    while (current != NULL) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");
}

int main(void) {
    struct Node *head = NULL;

    head = createNode(1);
    head->next = createNode(2);
    head->next->next = createNode(3);
    head->next->next->next = createNode(4);

    printf("Original list: ");
    printList(head);

    insertBeforeEach(&head, 99);

    printf("Modified list: ");
    printList(head);

    freeList(head);

    return 0;
}