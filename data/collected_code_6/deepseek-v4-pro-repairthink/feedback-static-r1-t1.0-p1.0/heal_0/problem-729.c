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
    Node* current1 = list1;
    Node* current2 = list2;
    int carry = 0;
    
    while (current1 != NULL || current2 != NULL || carry != 0) {
        int sum = carry;
        
        if (current1 != NULL) {
            sum += current1->data;
            current1 = current1->next;
        }
        
        if (current2 != NULL) {
            sum += current2->data;
            current2 = current2->next;
        }
        
        carry = sum / 10;
        Node* newNode = createNode(sum % 10);
        
        if (newNode == NULL) {
            Node* temp = result;
            while (temp != NULL) {
                Node* next = temp->next;
                free(temp);
                temp = next;
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

void freeList(Node* head) {
    Node* current = head;
    while (current != NULL) {
        Node* temp = current;
        current = current->next;
        free(temp);
    }
}

void printList(Node* head) {
    Node* current = head;
    while (current != NULL) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");
}

Node* appendNode(Node* head, int data) {
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

int main(void) {
    Node* list1 = NULL;
    Node* list2 = NULL;
    Node* result = NULL;
    
    list1 = appendNode(list1, 2);
    if (list1 == NULL) {
        return EXIT_FAILURE;
    }
    list1 = appendNode(list1, 4);
    if (list1 == NULL) {
        return EXIT_FAILURE;
    }
    list1 = appendNode(list1, 3);
    if (list1 == NULL) {
        return EXIT_FAILURE;
    }
    
    list2 = appendNode(list2, 5);
    if (list2 == NULL) {
        freeList(list1);
        return EXIT_FAILURE;
    }
    list2 = appendNode(list2, 6);
    if (list2 == NULL) {
        freeList(list1);
        return EXIT_FAILURE;
    }
    list2 = appendNode(list2, 4);
    if (list2 == NULL) {
        freeList(list1);
        return EXIT_FAILURE;
    }
    
    printf("List 1: ");
    printList(list1);
    
    printf("List 2: ");
    printList(list2);
    
    result = addLists(list1, list2);
    if (result == NULL) {
        freeList(list1);
        freeList(list2);
        return EXIT_FAILURE;
    }
    
    printf("Result: ");
    printList(result);
    
    freeList(list1);
    freeList(list2);
    freeList(result);
    
    return EXIT_SUCCESS;
}