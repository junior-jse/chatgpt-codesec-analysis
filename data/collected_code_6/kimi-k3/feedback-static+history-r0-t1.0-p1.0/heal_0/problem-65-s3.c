#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

int listSum(const Node *head)
{
    if (head == NULL) {
        return 0;
    }
    return head->data + listSum(head->next);
}

Node *createNode(int data)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL) {
        return NULL;
    }
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

void appendNode(Node **head, int data)
{
    Node *newNode = createNode(data);
    if (newNode == NULL) {
        return;
    }
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

void freeList(Node *head)
{
    Node *current = head;
    while (current != NULL) {
        Node *next = current->next;
        free(current);
        current = next;
    }
}

int main(void)
{
    Node *head = NULL;
    int values[] = {1, 2, 3, 4, 5};
    size_t count = sizeof(values) / sizeof(values[0]);
    size_t i;

    for (i = 0; i < count; i++) {
        appendNode(&head, values[i]);
    }

    printf("Sum: %d\n", listSum(head));

    freeList(head);
    return 0;
}