#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node* next;
} Node;

typedef struct List {
    Node* head;
    struct List* next;
} List;

List* create_list(void) {
    List* list = (List*)malloc(sizeof(List));
    if (list == NULL) {
        return NULL;
    }
    list->head = NULL;
    list->next = NULL;
    return list;
}

void add_node(List* list, int value) {
    if (list == NULL) {
        return;
    }
    Node* node = (Node*)malloc(sizeof(Node));
    if (node == NULL) {
        return;
    }
    node->data = value;
    node->next = list->head;
    list->head = node;
}

void add_list(List** head, List* list) {
    if (head == NULL || list == NULL) {
        return;
    }
    list->next = *head;
    *head = list;
}

int count_lists(List* head) {
    int count = 0;
    List* current = head;
    while (current != NULL) {
        count++;
        current = current->next;
    }
    return count;
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

void free_all_lists(List* head) {
    List* current = head;
    while (current != NULL) {
        List* temp = current;
        current = current->next;
        free_list(temp);
    }
}

int main(void) {
    List* list_head = NULL;
    
    List* list1 = create_list();
    List* list2 = create_list();
    List* list3 = create_list();
    
    if (list1 == NULL || list2 == NULL || list3 == NULL) {
        free_list(list1);
        free_list(list2);
        free_list(list3);
        return 1;
    }
    
    add_node(list1, 10);
    add_node(list1, 20);
    add_node(list2, 30);
    add_node(list3, 40);
    add_node(list3, 50);
    add_node(list3, 60);
    
    add_list(&list_head, list1);
    add_list(&list_head, list2);
    add_list(&list_head, list3);
    
    int num_lists = count_lists(list_head);
    printf("Number of lists: %d\n", num_lists);
    
    free_all_lists(list_head);
    
    return 0;
}