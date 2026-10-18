#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

typedef struct List {
    Node *head;
    struct List *next;
} List;

Node *createNode(int data) {
    Node *newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

List *createList() {
    List *newList = (List*)malloc(sizeof(List));
    if (newList == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    newList->head = NULL;
    newList->next = NULL;
    return newList;
}

void appendNode(List *list, int data) {
    Node *newNode = createNode(data);
    if (list->head == NULL) {
        list->head = newNode;
    } else {
        Node *current = list->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
}

void appendList(List **head, List *list) {
    if (*head == NULL) {
        *head = list;
    } else {
        List *current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = list;
    }
}

void freeList(List *list) {
    Node *current = list->head;
    while (current != NULL) {
        Node *temp = current;
        current = current->next;
        free(temp);
    }
    free(list);
}

void freeListOfLists(List *head) {
    List *current = head;
    while (current != NULL) {
        List *temp = current;
        current = current->next;
        freeList(temp);
    }
}

int listsEqual(Node *head1, Node *head2) {
    Node *current1 = head1;
    Node *current2 = head2;
    
    while (current1 != NULL && current2 != NULL) {
        if (current1->data != current2->data) {
            return 0;
        }
        current1 = current1->next;
        current2 = current2->next;
    }
    
    return current1 == NULL && current2 == NULL;
}

List *removeDuplicates(List *head) {
    if (head == NULL || head->next == NULL) {
        return head;
    }
    
    List *result = NULL;
    List *current = head;
    
    while (current != NULL) {
        int isDuplicate = 0;
        List *check = result;
        
        while (check != NULL) {
            if (listsEqual(current->head, check->head)) {
                isDuplicate = 1;
                break;
            }
            check = check->next;
        }
        
        if (!isDuplicate) {
            List *newList = createList();
            Node *node = current->head;
            while (node != NULL) {
                appendNode(newList, node->data);
                node = node->next;
            }
            appendList(&result, newList);
        }
        
        current = current->next;
    }
    
    freeListOfLists(head);
    return result;
}

void printListOfLists(List *head) {
    List *current = head;
    while (current != NULL) {
        Node *node = current->head;
        printf("[");
        while (node != NULL) {
            printf("%d", node->data);
            if (node->next != NULL) {
                printf(", ");
            }
            node = node->next;
        }
        printf("]");
        if (current->next != NULL) {
            printf(" -> ");
        }
        current = current->next;
    }
    printf("\n");
}

int main() {
    List *listOfLists = NULL;
    
    List *list1 = createList();
    appendNode(list1, 1);
    appendNode(list1, 2);
    appendNode(list1, 3);
    appendList(&listOfLists, list1);
    
    List *list2 = createList();
    appendNode(list2, 4);
    appendNode(list2, 5);
    appendList(&listOfLists, list2);
    
    List *list3 = createList();
    appendNode(list3, 1);
    appendNode(list3, 2);
    appendNode(list3, 3);
    appendList(&listOfLists, list3);
    
    List *list4 = createList();
    appendNode(list4, 6);
    appendList(&listOfLists, list4);
    
    List *list5 = createList();
    appendNode(list5, 4);
    appendNode(list5, 5);
    appendList(&listOfLists, list5);
    
    printf("Original list of lists: ");
    printListOfLists(listOfLists);
    
    listOfLists = removeDuplicates(listOfLists);
    
    printf("After removing duplicates: ");
    printListOfLists(listOfLists);
    
    freeListOfLists(listOfLists);
    
    return 0;
}