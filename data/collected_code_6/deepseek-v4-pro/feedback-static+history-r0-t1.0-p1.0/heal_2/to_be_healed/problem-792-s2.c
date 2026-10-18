#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node* next;
};

struct List {
    struct Node* head;
};

int countLists(struct List** lists, int numLists) {
    int count = 0;
    
    if (lists == NULL) {
        return 0;
    }
    
    for (int i = 0; i < numLists; i++) {
        if (lists[i] != NULL) {
            count++;
        }
    }
    
    return count;
}

struct List* createList(const int* arr, int size) {
    if (arr == NULL || size <= 0) {
        return NULL;
    }
    
    struct List* list = (struct List*)malloc(sizeof(struct List));
    if (list == NULL) {
        return NULL;
    }
    
    list->head = NULL;
    
    for (int i = size - 1; i >= 0; i--) {
        struct Node* node = (struct Node*)malloc(sizeof(struct Node));
        if (node == NULL) {
            struct Node* current = list->head;
            while (current != NULL) {
                struct Node* temp = current;
                current = current->next;
                free(temp);
            }
            free(list);
            return NULL;
        }
        node->data = arr[i];
        node->next = list->head;
        list->head = node;
    }
    
    return list;
}

void freeList(struct List* list) {
    if (list == NULL) {
        return;
    }
    
    struct Node* current = list->head;
    while (current != NULL) {
        struct Node* temp = current;
        current = current->next;
        free(temp);
    }
    
    free(list);
}

int main(void) {
    /* Possible weaknesses found:
     *  Variable 'arr1' can be declared as const array [constVariable]
     */
    int arr1[] = {1, 2, 3};
    /* Possible weaknesses found:
     *  Variable 'arr2' can be declared as const array [constVariable]
     */
    int arr2[] = {4, 5};
    
    struct List* list1 = createList(arr1, 3);
    struct List* list2 = createList(arr2, 2);
    struct List* list3 = NULL;
    
    struct List* lists[] = {list1, list2, list3, NULL, list1};
    
    int numLists = sizeof(lists) / sizeof(lists[0]);
    int listCount = countLists(lists, numLists);
    
    printf("Number of non-NULL lists: %d\n", listCount);
    
    freeList(list1);
    freeList(list2);
    
    return 0;
}