#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int *data;
    int size;
    struct Node *next;
} Node;

int countSublistsContainingElement(Node *head, int element) {
    int count = 0;
    Node *current = head;
    
    while (current != NULL) {
        if (current->data != NULL) {
            for (int i = 0; i < current->size; i++) {
                if (current->data[i] == element) {
                    count++;
                    break;
                }
            }
        }
        current = current->next;
    }
    
    return count;
}

/* Possible weaknesses found:
 *  Parameter 'data' can be declared as pointer to const [constParameterPointer]
 */
Node* createNode(int *data, int size) {
    Node *newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        return NULL;
    }
    
    newNode->data = (int*)malloc(size * sizeof(int));
    if (newNode->data == NULL) {
        free(newNode);
        return NULL;
    }
    
    for (int i = 0; i < size; i++) {
        newNode->data[i] = data[i];
    }
    
    newNode->size = size;
    newNode->next = NULL;
    
    return newNode;
}

void appendNode(Node **head, int *data, int size) {
    Node *newNode = createNode(data, size);
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

void freeList(Node *head) {
    Node *current = head;
    Node *next;
    
    while (current != NULL) {
        next = current->next;
        if (current->data != NULL) {
            free(current->data);
        }
        free(current);
        current = next;
    }
}

int main(void) {
    Node *head = NULL;
    
    int list1[] = {1, 2, 3, 4, 5};
    int list2[] = {6, 7, 8};
    int list3[] = {1, 9, 10};
    int list4[] = {11, 12, 1, 13};
    
    appendNode(&head, list1, 5);
    appendNode(&head, list2, 3);
    appendNode(&head, list3, 3);
    appendNode(&head, list4, 4);
    
    int element = 1;
    int count = countSublistsContainingElement(head, element);
    
    printf("Number of sublists containing %d: %d\n", element, count);
    
    freeList(head);
    
    return 0;
}