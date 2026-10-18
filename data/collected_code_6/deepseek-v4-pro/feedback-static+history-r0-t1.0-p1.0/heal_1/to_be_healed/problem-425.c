#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

typedef struct {
    Node *head;
    size_t size;
} LinkedList;

LinkedList *create_list(void) {
    LinkedList *list = malloc(sizeof(LinkedList));
    if (!list) return NULL;
    list->head = NULL;
    list->size = 0;
    return list;
}

int append(LinkedList *list, int value) {
    if (!list) return -1;
    Node *new_node = malloc(sizeof(Node));
    if (!new_node) return -1;
    new_node->data = value;
    new_node->next = NULL;
    if (!list->head) {
        list->head = new_node;
    } else {
        Node *current = list->head;
        while (current->next) current = current->next;
        current->next = new_node;
    }
    list->size++;
    return 0;
}

void destroy_list(LinkedList *list) {
    if (!list) return;
    Node *current = list->head;
    while (current) {
        Node *next = current->next;
        free(current);
        current = next;
    }
    free(list);
}

int count_sublists_containing(LinkedList *list, int element) {
    if (!list || !list->head) return 0;
    
    int count = 0;
    Node *start = list->head;
    size_t start_index = 0;
    
    while (start) {
        Node *current = start;
        int found = 0;
        while (current) {
            if (current->data == element) {
                found = 1;
                break;
            }
            current = current->next;
        }
        if (found) {
            count += (int)(list->size - start_index);
        }
        start = start->next;
        start_index++;
    }
    
    return count;
}

int main(void) {
    LinkedList *list = create_list();
    if (!list) return 1;
    
    /* Possible weaknesses found:
     *  Variable 'values' can be declared as const array [constVariable]
     */
    int values[] = {1, 2, 3, 2, 1};
    for (int i = 0; i < 5; i++) {
        if (append(list, values[i]) != 0) {
            destroy_list(list);
            return 1;
        }
    }
    
    int element = 2;
    int result = count_sublists_containing(list, element);
    printf("%d\n", result);
    
    destroy_list(list);
    return 0;
}