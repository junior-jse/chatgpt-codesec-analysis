#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Node {
    int data;
    struct Node* next;
} Node;

typedef struct {
    Node* head;
    Node* tail;
    int size;
} LinkedList;

LinkedList* create_list(void) {
    LinkedList* list = malloc(sizeof(LinkedList));
    if (list == NULL) {
        return NULL;
    }
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return list;
}

bool append(LinkedList* list, int data) {
    if (list == NULL) {
        return false;
    }
    
    Node* new_node = malloc(sizeof(Node));
    if (new_node == NULL) {
        return false;
    }
    
    new_node->data = data;
    new_node->next = NULL;
    
    if (list->tail == NULL) {
        list->head = new_node;
        list->tail = new_node;
    } else {
        list->tail->next = new_node;
        list->tail = new_node;
    }
    list->size++;
    return true;
}

bool contains(const LinkedList* list, int target) {
    if (list == NULL) {
        return false;
    }
    
    const Node* current = list->head;
    while (current != NULL) {
        if (current->data == target) {
            return true;
        }
        current = current->next;
    }
    return false;
}

int count_sublists_containing(LinkedList** lists, int num_lists, int target) {
    if (lists == NULL || num_lists < 0) {
        return -1;
    }
    
    int count = 0;
    for (int i = 0; i < num_lists; i++) {
        if (lists[i] != NULL && contains(lists[i], target)) {
            count++;
        }
    }
    return count;
}

void free_list(LinkedList* list) {
    if (list == NULL) {
        return;
    }
    
    Node* current = list->head;
    while (current != NULL) {
        Node* temp = current;
        current = current->next;
        free(temp);
    }
    free(list);
}

int main(void) {
    LinkedList* lists[3] = {NULL, NULL, NULL};
    
    lists[0] = create_list();
    lists[1] = create_list();
    lists[2] = create_list();
    
    if (lists[0] == NULL || lists[1] == NULL || lists[2] == NULL) {
        for (int i = 0; i < 3; i++) {
            free_list(lists[i]);
        }
        return EXIT_FAILURE;
    }
    
    if (!append(lists[0], 1) || !append(lists[0], 2) || !append(lists[0], 3)) {
        for (int i = 0; i < 3; i++) {
            free_list(lists[i]);
        }
        return EXIT_FAILURE;
    }
    
    if (!append(lists[1], 4) || !append(lists[1], 5)) {
        for (int i = 0; i < 3; i++) {
            free_list(lists[i]);
        }
        return EXIT_FAILURE;
    }
    
    if (!append(lists[2], 2) || !append(lists[2], 6)) {
        for (int i = 0; i < 3; i++) {
            free_list(lists[i]);
        }
        return EXIT_FAILURE;
    }
    
    int target = 2;
    int result = count_sublists_containing(lists, 3, target);
    
    if (result >= 0) {
        printf("Number of sublists containing %d: %d\n", target, result);
    } else {
        printf("Error occurred\n");
    }
    
    for (int i = 0; i < 3; i++) {
        free_list(lists[i]);
    }
    
    return EXIT_SUCCESS;
}