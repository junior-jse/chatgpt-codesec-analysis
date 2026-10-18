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

Node* addLists(Node* list1, Node* list2) {
    Node* result = NULL;
    Node* tail = NULL;
    int carry = 0;

    while (list1 != NULL || list2 != NULL || carry != 0) {
        int sum = carry;
        if (list1 != NULL) {
            sum += list1->data;
            list1 = list1->next;
        }
        if (list2 != NULL) {
            sum += list2->data;
            list2 = list2->next;
        }
        carry = sum / 10;
        int digit = sum % 10;

        Node* newNode = createNode(digit);
        if (newNode == NULL) {
            while (result != NULL) {
                Node* temp = result;
                result = result->next;
                free(temp);
            }
            return NULL;
        }

        if (result == NULL) {
            result = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }

    return result;
}

void printList(Node* head) {
    while (head != NULL) {
        printf("%d", head->data);
        if (head->next != NULL) {
            printf(" -> ");
        }
        head = head->next;
    }
    printf("\n");
}

void freeList(Node* head) {
    while (head != NULL) {
        Node* temp = head;
        head = head->next;
        free(temp);
    }
}

int main(void) {
    Node* list1 = createNode(2);
    if (list1 == NULL) {
        return EXIT_FAILURE;
    }
    list1->next = createNode(4);
    if (list1->next == NULL) {
        freeList(list1);
        return EXIT_FAILURE;
    }
    list1->next->next = createNode(3);
    if (list1->next->next == NULL) {
        freeList(list1);
        return EXIT_FAILURE;
    }

    Node* list2 = createNode(5);
    if (list2 == NULL) {
        freeList(list1);
        return EXIT_FAILURE;
    }
    list2->next = createNode(6);
    if (list2->next == NULL) {
        freeList(list1);
        freeList(list2);
        return EXIT_FAILURE;
    }
    list2->next->next = createNode(4);
    if (list2->next->next == NULL) {
        freeList(list1);
        freeList(list2);
        return EXIT_FAILURE;
    }

    Node* result = addLists(list1, list2);
    if (result == NULL) {
        freeList(list1);
        freeList(list2);
        return EXIT_FAILURE;
    }

    printf("List 1: ");
    printList(list1);
    printf("List 2: ");
    printList(list2);
    printf("Sum: ");
    printList(result);

    freeList(list1);
    freeList(list2);
    freeList(result);

    return EXIT_SUCCESS;
}