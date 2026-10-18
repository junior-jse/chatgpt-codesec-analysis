#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node* next;
} Node;

typedef struct {
    Node* head;
    int length;
} SublistResult;

Node* createNode(int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        return NULL;
    }
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

void freeList(Node* head) {
    Node* current = head;
    Node* nextNode;
    while (current != NULL) {
        nextNode = current->next;
        free(current);
        current = nextNode;
    }
}

SublistResult findMinLengthSublist(Node* head, int targetSum) {
    SublistResult result = {NULL, -1};
    
    if (head == NULL) {
        return result;
    }
    
    Node* start = head;
    int minLength = -1;
    Node* minStart = NULL;
    
    while (start != NULL) {
        Node* end = start;
        int currentSum = 0;
        int currentLength = 0;
        
        while (end != NULL) {
            currentSum += end->data;
            currentLength++;
            
            if (currentSum == targetSum) {
                if (minLength == -1 || currentLength < minLength) {
                    minLength = currentLength;
                    minStart = start;
                }
                break;
            }
            end = end->next;
        }
        start = start->next;
    }
    
    if (minStart != NULL) {
        result.head = minStart;
        result.length = minLength;
    }
    
    return result;
}

void printSublist(Node* head, int length) {
    Node* current = head;
    int count = 0;
    while (current != NULL && count < length) {
        printf("%d", current->data);
        if (count < length - 1) {
            printf(" -> ");
        }
        current = current->next;
        count++;
    }
    printf("\n");
}

int main(void) {
    Node* head = NULL;
    Node* tail = NULL;
    int values[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int n = sizeof(values) / sizeof(values[0]);
    int targetSum = 15;
    
    for (int i = 0; i < n; i++) {
        Node* newNode = createNode(values[i]);
        if (newNode == NULL) {
            freeList(head);
            return EXIT_FAILURE;
        }
        
        if (head == NULL) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }
    
    SublistResult result = findMinLengthSublist(head, targetSum);
    
    if (result.head != NULL) {
        printf("Minimum length sublist with sum %d: ", targetSum);
        printSublist(result.head, result.length);
        printf("Length: %d\n", result.length);
    } else {
        printf("No sublist found with sum %d\n", targetSum);
    }
    
    freeList(head);
    return EXIT_SUCCESS;
}