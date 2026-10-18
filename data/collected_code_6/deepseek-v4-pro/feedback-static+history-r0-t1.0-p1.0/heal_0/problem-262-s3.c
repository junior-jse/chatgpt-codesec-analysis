#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node* next;
};

void splitList(struct Node* source, struct Node** frontRef, struct Node** backRef, int splitIndex) {
    struct Node* current = source;
    int count = 0;

    if (source == NULL) {
        *frontRef = NULL;
        *backRef = NULL;
        return;
    }

    if (splitIndex <= 0) {
        *frontRef = NULL;
        *backRef = source;
        return;
    }

    while (current != NULL && count < splitIndex - 1) {
        current = current->next;
        count++;
    }

    if (current == NULL) {
        *frontRef = source;
        *backRef = NULL;
        return;
    }

    *frontRef = source;
    *backRef = current->next;
    current->next = NULL;
}

struct Node* newNode(int data) {
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    if (node == NULL) {
        exit(1);
    }
    node->data = data;
    node->next = NULL;
    return node;
}

void freeList(struct Node* head) {
    struct Node* tmp;
    while (head != NULL) {
        tmp = head;
        head = head->next;
        free(tmp);
    }
}

void printList(struct Node* head) {
    struct Node* current = head;
    while (current != NULL) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");
}

int main() {
    struct Node* head = NULL;
    struct Node* front = NULL;
    struct Node* back = NULL;
    int i;

    for (i = 1; i <= 10; i++) {
        struct Node* temp = newNode(i);
        if (head == NULL) {
            head = temp;
        } else {
            struct Node* last = head;
            while (last->next != NULL) {
                last = last->next;
            }
            last->next = temp;
        }
    }

    printf("Original list: ");
    printList(head);

    splitList(head, &front, &back, 4);

    printf("First part: ");
    printList(front);
    printf("Second part: ");
    printList(back);

    freeList(front);
    freeList(back);

    return 0;
}