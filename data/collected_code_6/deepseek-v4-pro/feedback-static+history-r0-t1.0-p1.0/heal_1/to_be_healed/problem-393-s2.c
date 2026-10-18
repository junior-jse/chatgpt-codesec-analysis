#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

typedef struct List {
    Node *head;
    int length;
} List;

List *findMaxLengthList(List **lists, int numLists) {
    if (lists == NULL || numLists <= 0) {
        return NULL;
    }

    List *maxList = lists[0];
    int maxLength = maxList ? maxList->length : 0;

    for (int i = 1; i < numLists; i++) {
        if (lists[i] != NULL && lists[i]->length > maxLength) {
            maxList = lists[i];
            maxLength = lists[i]->length;
        }
    }

    return maxList;
}

void appendNode(List *list, int data) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    newNode->data = data;
    newNode->next = NULL;

    if (list->head == NULL) {
        list->head = newNode;
    } else {
        Node *temp = list->head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newNode;
    }
    list->length++;
}

List *createList(void) {
    List *list = (List *)malloc(sizeof(List));
    if (list == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    list->head = NULL;
    list->length = 0;
    return list;
}

void freeList(List *list) {
    if (list == NULL) {
        return;
    }
    Node *current = list->head;
    while (current != NULL) {
        Node *next = current->next;
        free(current);
        current = next;
    }
    free(list);
}

int main(void) {
    List *list1 = createList();
    List *list2 = createList();
    List *list3 = createList();

    appendNode(list1, 10);
    appendNode(list1, 20);
    appendNode(list1, 30);

    appendNode(list2, 1);

    appendNode(list3, 5);
    appendNode(list3, 15);
    appendNode(list3, 25);
    appendNode(list3, 35);
    appendNode(list3, 45);

    List *lists[] = {list1, list2, list3};
    int numLists = 3;

    /* Possible weaknesses found:
     *  Variable 'maxList' can be declared as pointer to const [constVariablePointer]
     */
    List *maxList = findMaxLengthList(lists, numLists);

    if (maxList != NULL) {
        printf("List with maximum length has length: %d\n", maxList->length);
    } else {
        printf("No valid list found\n");
    }

    freeList(list1);
    freeList(list2);
    freeList(list3);

    return 0;
}