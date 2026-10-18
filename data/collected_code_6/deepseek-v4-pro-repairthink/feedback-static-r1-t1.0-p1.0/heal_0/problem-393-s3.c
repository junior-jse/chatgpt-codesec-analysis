#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node* next;
} Node;

typedef struct List {
    Node* head;
    int length;
} List;

List* create_list() {
    List* list = (List*)malloc(sizeof(List));
    if (list == NULL) {
        return NULL;
    }
    list->head = NULL;
    list->length = 0;
    return list;
}

void append_node(List* list, int data) {
    if (list == NULL) {
        return;
    }
    
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL) {
        return;
    }
    new_node->data = data;
    new_node->next = NULL;
    
    if (list->head == NULL) {
        list->head = new_node;
    } else {
        Node* current = list->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
    }
    list->length++;
}

void free_list(List* list) {
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

List* find_max_length_list(List** lists, int count) {
    if (lists == NULL || count <= 0) {
        return NULL;
    }
    
    List* max_list = lists[0];
    int max_length = (max_list != NULL) ? max_list->length : 0;
    
    for (int i = 1; i < count; i++) {
        if (lists[i] != NULL && lists[i]->length > max_length) {
            max_list = lists[i];
            max_length = lists[i]->length;
        }
    }
    
    return max_list;
}

int main(void) {
    List* lists[3];
    
    lists[0] = create_list();
    lists[1] = create_list();
    lists[2] = create_list();
    
    if (lists[0] == NULL || lists[1] == NULL || lists[2] == NULL) {
        free_list(lists[0]);
        free_list(lists[1]);
        free_list(lists[2]);
        return 1;
    }
    
    append_node(lists[0], 1);
    append_node(lists[0], 2);
    
    append_node(lists[1], 3);
    append_node(lists[1], 4);
    append_node(lists[1], 5);
    append_node(lists[1], 6);
    
    append_node(lists[2], 7);
    append_node(lists[2], 8);
    append_node(lists[2], 9);
    
    List* max_list = find_max_length_list(lists, 3);
    
    if (max_list != NULL) {
        printf("Maximum length list has %d nodes\n", max_list->length);
    }
    
    for (int i = 0; i < 3; i++) {
        free_list(lists[i]);
    }
    
    return 0;
}