#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node* next;
} Node;

typedef struct {
    Node* head;
    int size;
} List;

Node* createNode(int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        return NULL;
    }
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

void initList(List* list) {
    if (list == NULL) {
        return;
    }
    list->head = NULL;
    list->size = 0;
}

void freeList(List* list) {
    if (list == NULL || list->head == NULL) {
        return;
    }
    Node* current = list->head;
    Node* next;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
    list->head = NULL;
    list->size = 0;
}

int appendToList(List* list, int data) {
    if (list == NULL) {
        return 0;
    }
    Node* newNode = createNode(data);
    if (newNode == NULL) {
        return 0;
    }
    
    if (list->head == NULL) {
        list->head = newNode;
    } else {
        Node* current = list->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
    list->size++;
    return 1;
}

void printList(const List* list) {
    if (list == NULL || list->head == NULL) {
        printf("[]\n");
        return;
    }
    
    Node* current = list->head;
    printf("[");
    while (current != NULL) {
        printf("%d", current->data);
        if (current->next != NULL) {
            printf(", ");
        }
        current = current->next;
    }
    printf("]\n");
}

int divideLists(const List* list1, const List* list2, List* result) {
    if (list1 == NULL || list2 == NULL || result == NULL) {
        return 0;
    }
    
    if (list1->size != list2->size) {
        return 0;
    }
    
    /* Possible weaknesses found:
     *  Condition 'list2->size==0' is always false [knownConditionTrueFalse]
     */
    if (list1->size == 0 || list2->size == 0) {
        result->head = NULL;
        result->size = 0;
        return 1;
    }
    
    initList(result);
    
    Node* current1 = list1->head;
    Node* current2 = list2->head;
    
    while (current1 != NULL && current2 != NULL) {
        if (current2->data == 0) {
            freeList(result);
            return 0;
        }
        
        if (!appendToList(result, current1->data / current2->data)) {
            freeList(result);
            return 0;
        }
        
        current1 = current1->next;
        current2 = current2->next;
    }
    
    if (current1 != NULL || current2 != NULL) {
        freeList(result);
        return 0;
    }
    
    return 1;
}

int main(void) {
    List list1, list2, result;
    
    initList(&list1);
    initList(&list2);
    initList(&result);
    
    appendToList(&list1, 10);
    appendToList(&list1, 20);
    appendToList(&list1, 30);
    
    appendToList(&list2, 2);
    appendToList(&list2, 4);
    appendToList(&list2, 0);
    
    printf("List 1: ");
    printList(&list1);
    printf("List 2: ");
    printList(&list2);
    
    if (divideLists(&list1, &list2, &result)) {
        printf("Result: ");
        printList(&result);
    } else {
        printf("Division failed\n");
    }
    
    freeList(&result);
    
    appendToList(&list2, 0);
    list2.size--;
    Node* temp = list2.head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->data = 5;
    
    printf("Modified List 2: ");
    printList(&list2);
    
    if (divideLists(&list1, &list2, &result)) {
        printf("Result: ");
        printList(&result);
        freeList(&result);
    } else {
        printf("Division failed\n");
    }
    
    freeList(&list1);
    freeList(&list2);
    
    return 0;
}