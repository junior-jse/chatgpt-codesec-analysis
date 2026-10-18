#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node* next;
};

struct Node* createNode(int data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    if (newNode == NULL) {
        return NULL;
    }
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

struct Node* mergeSortedLists(struct Node* list1, struct Node* list2) {
    struct Node* mergedHead = NULL;
    struct Node* mergedTail = NULL;
    struct Node* temp1 = list1;
    struct Node* temp2 = list2;

    while (temp1 != NULL && temp2 != NULL) {
        struct Node* newNode;
        if (temp1->data <= temp2->data) {
            newNode = createNode(temp1->data);
            temp1 = temp1->next;
        } else {
            newNode = createNode(temp2->data);
            temp2 = temp2->next;
        }
        if (newNode == NULL) {
            while (mergedHead != NULL) {
                struct Node* toFree = mergedHead;
                mergedHead = mergedHead->next;
                free(toFree);
            }
            return NULL;
        }
        if (mergedHead == NULL) {
            mergedHead = newNode;
            mergedTail = newNode;
        } else {
            mergedTail->next = newNode;
            mergedTail = newNode;
        }
    }

    while (temp1 != NULL) {
        struct Node* newNode = createNode(temp1->data);
        if (newNode == NULL) {
            while (mergedHead != NULL) {
                struct Node* toFree = mergedHead;
                mergedHead = mergedHead->next;
                free(toFree);
            }
            return NULL;
        }
        if (mergedHead == NULL) {
            mergedHead = newNode;
            mergedTail = newNode;
        } else {
            mergedTail->next = newNode;
            mergedTail = newNode;
        }
        temp1 = temp1->next;
    }

    while (temp2 != NULL) {
        struct Node* newNode = createNode(temp2->data);
        if (newNode == NULL) {
            while (mergedHead != NULL) {
                struct Node* toFree = mergedHead;
                mergedHead = mergedHead->next;
                free(toFree);
            }
            return NULL;
        }
        if (mergedHead == NULL) {
            mergedHead = newNode;
            mergedTail = newNode;
        } else {
            mergedTail->next = newNode;
            mergedTail = newNode;
        }
        temp2 = temp2->next;
    }

    return mergedHead;
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
    while (head != NULL) {
        struct Node* temp = head;
        head = head->next;
        free(temp);
    }
}

int main(void) {
    struct Node* list1 = createNode(1);
    if (list1 == NULL) {
        return 1;
    }
    list1->next = createNode(3);
    if (list1->next == NULL) {
        freeList(list1);
        return 1;
    }
    list1->next->next = createNode(5);
    if (list1->next->next == NULL) {
        freeList(list1);
        return 1;
    }

    struct Node* list2 = createNode(2);
    if (list2 == NULL) {
        freeList(list1);
        return 1;
    }
    list2->next = createNode(4);
    if (list2->next == NULL) {
        freeList(list2);
        freeList(list1);
        return 1;
    }
    list2->next->next = createNode(6);
    if (list2->next->next == NULL) {
        freeList(list2);
        freeList(list1);
        return 1;
    }

    struct Node* merged = mergeSortedLists(list1, list2);
    if (merged == NULL) {
        freeList(list1);
        freeList(list2);
        return 1;
    }

    printList(merged);

    freeList(list1);
    freeList(list2);
    freeList(merged);

    return 0;
}