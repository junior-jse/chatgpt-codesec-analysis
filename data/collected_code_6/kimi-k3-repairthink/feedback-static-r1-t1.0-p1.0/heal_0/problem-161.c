#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node* next;
} Node;

void removeElements(Node** head, Node* removeList) {
    Node* current = *head;
    Node* prev = NULL;

    while (current != NULL) {
        Node* temp = removeList;
        int found = 0;
        while (temp != NULL) {
            if (current->data == temp->data) {
                found = 1;
                break;
            }
            temp = temp->next;
        }

        if (found) {
            if (prev == NULL) {
                *head = current->next;
            } else {
                prev->next = current->next;
            }
            free(current);
            current = (prev == NULL) ? *head : prev->next;
        } else {
            prev = current;
            current = current->next;
        }
    }
}

void append(Node** head, int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->next = NULL;

    if (*head == NULL) {
        *head = newNode;
        return;
    }

    Node* temp = *head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = newNode;
}

void printList(Node* head) {
    while (head != NULL) {
        printf("%d ", head->data);
        head = head->next;
    }
    printf("\n");
}

void freeList(Node* head) {
    Node* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

int main() {
    Node* list1 = NULL;
    Node* list2 = NULL;

    append(&list1, 1);
    append(&list1, 2);
    append(&list1, 3);
    append(&list1, 4);
    append(&list1, 5);

    append(&list2, 2);
    append(&list2, 4);

    printf("Original list: ");
    printList(list1);

    removeElements(&list1, list2);

    printf("List after removal: ");
    printList(list1);

    freeList(list1);
    freeList(list2);

    return 0;
}