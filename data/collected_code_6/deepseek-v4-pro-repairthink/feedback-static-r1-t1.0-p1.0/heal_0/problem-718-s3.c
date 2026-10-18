#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node* next;
};

struct Node* createAlternateList(struct Node* head) {
    if (head == NULL) {
        return NULL;
    }

    struct Node* newHead = NULL;
    struct Node* newTail = NULL;
    struct Node* current = head;
    int take = 1;

    while (current != NULL) {
        if (take) {
            struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
            if (newNode == NULL) {
                struct Node* temp = newHead;
                while (temp != NULL) {
                    struct Node* next = temp->next;
                    free(temp);
                    temp = next;
                }
                return NULL;
            }
            newNode->data = current->data;
            newNode->next = NULL;

            if (newHead == NULL) {
                newHead = newNode;
                newTail = newNode;
            } else {
                newTail->next = newNode;
                newTail = newNode;
            }
        }
        take = !take;
        current = current->next;
    }

    return newHead;
}

struct Node* insertAtEnd(struct Node* head, int data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    if (newNode == NULL) {
        return head;
    }
    newNode->data = data;
    newNode->next = NULL;

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

void printList(struct Node* head) {
    struct Node* temp = head;
    while (temp != NULL) {
        printf("%d ", temp->data);
        temp = temp->next;
    }
    printf("\n");
}

void freeList(struct Node* head) {
    struct Node* temp = head;
    while (temp != NULL) {
        struct Node* next = temp->next;
        free(temp);
        temp = next;
    }
}

int main(void) {
    struct Node* original = NULL;
    struct Node* alternate = NULL;

    original = insertAtEnd(original, 1);
    original = insertAtEnd(original, 2);
    original = insertAtEnd(original, 3);
    original = insertAtEnd(original, 4);
    original = insertAtEnd(original, 5);
    original = insertAtEnd(original, 6);

    printf("Original list: ");
    printList(original);

    alternate = createAlternateList(original);

    printf("Alternate list: ");
    printList(alternate);

    freeList(original);
    freeList(alternate);

    return 0;
}