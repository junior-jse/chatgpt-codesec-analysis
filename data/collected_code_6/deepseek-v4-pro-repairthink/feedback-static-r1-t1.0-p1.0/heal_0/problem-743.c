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

void appendNode(struct Node **head, int data) {
    struct Node *newNode = createNode(data);
    if (*head == NULL) {
        *head = newNode;
        return;
    }
    struct Node *current = *head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = newNode;
}

int getLength(struct Node *head) {
    int length = 0;
    struct Node *current = head;
    while (current != NULL) {
        length++;
        current = current->next;
    }
    return length;
}

struct Node *rotateRight(struct Node *head, int k) {
    if (head == NULL || head->next == NULL || k <= 0) {
        return head;
    }

    int length = getLength(head);
    k = k % length;
    if (k == 0) {
        return head;
    }

    struct Node *fast = head;
    struct Node *slow = head;

    for (int i = 0; i < k; i++) {
        fast = fast->next;
    }

    while (fast->next != NULL) {
        slow = slow->next;
        fast = fast->next;
    }

    struct Node *newHead = slow->next;
    slow->next = NULL;
    fast->next = head;
    return newHead;
}

void printList(struct Node *head) {
    struct Node *current = head;
    while (current != NULL) {
        printf("%d -> ", current->data);
        current = current->next;
    }
    printf("NULL\n");
}

void freeList(struct Node *head) {
    struct Node *current = head;
    while (current != NULL) {
        struct Node *temp = current;
        current = current->next;
        free(temp);
    }
}

int main() {
    struct Node *head = NULL;

    appendNode(&head, 1);
    appendNode(&head, 2);
    appendNode(&head, 3);
    appendNode(&head, 4);
    appendNode(&head, 5);

    printf("Original list: ");
    printList(head);

    int k = 2;
    head = rotateRight(head, k);

    printf("After rotating by %d positions to the right: ", k);
    printList(head);

    freeList(head);
    return 0;
}