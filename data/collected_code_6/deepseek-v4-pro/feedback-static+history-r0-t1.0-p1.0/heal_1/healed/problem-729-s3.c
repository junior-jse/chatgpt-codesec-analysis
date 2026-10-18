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

Node* addTwoLists(Node* first, Node* second) {
    Node* res = NULL;
    Node* temp = NULL;
    Node* prev = NULL;
    int carry = 0;

    while (first != NULL || second != NULL) {
        int sum = carry + (first ? first->data : 0) + (second ? second->data : 0);
        carry = (sum >= 10) ? 1 : 0;
        sum = sum % 10;
        temp = createNode(sum);
        if (temp == NULL) {
            while (res != NULL) {
                Node* next = res->next;
                free(res);
                res = next;
            }
            return NULL;
        }
        if (res == NULL) {
            res = temp;
        } else {
            prev->next = temp;
        }
        prev = temp;
        if (first) first = first->next;
        if (second) second = second->next;
    }

    if (carry > 0) {
        temp = createNode(carry);
        if (temp == NULL) {
            while (res != NULL) {
                Node* next = res->next;
                free(res);
                res = next;
            }
            return NULL;
        }
        prev->next = temp;
    }

    return res;
}

void printList(Node* node) {
    while (node != NULL) {
        printf("%d", node->data);
        if (node->next != NULL) {
            printf(" -> ");
        }
        node = node->next;
    }
    printf("\n");
}

void freeList(Node* node) {
    while (node != NULL) {
        Node* temp = node;
        node = node->next;
        free(temp);
    }
}

Node* insertAtEnd(Node* head, int data) {
    Node* newNode = createNode(data);
    if (newNode == NULL) {
        freeList(head);
        return NULL;
    }
    if (head == NULL) {
        return newNode;
    }
    Node* current = head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = newNode;
    return head;
}

int main() {
    Node* first = NULL;
    Node* second = NULL;
    Node* result = NULL;

    first = insertAtEnd(first, 7);
    if (first == NULL) return 1;
    first = insertAtEnd(first, 1);
    if (first == NULL) return 1;
    first = insertAtEnd(first, 6);
    if (first == NULL) return 1;

    second = insertAtEnd(second, 5);
    if (second == NULL) {
        freeList(first);
        return 1;
    }
    second = insertAtEnd(second, 9);
    if (second == NULL) {
        freeList(first);
        return 1;
    }
    second = insertAtEnd(second, 2);
    if (second == NULL) {
        freeList(first);
        return 1;
    }

    printf("First list: ");
    printList(first);
    printf("Second list: ");
    printList(second);

    result = addTwoLists(first, second);
    if (result == NULL) {
        freeList(first);
        freeList(second);
        return 1;
    }

    printf("Result list: ");
    printList(result);

    freeList(first);
    freeList(second);
    freeList(result);

    return 0;
}