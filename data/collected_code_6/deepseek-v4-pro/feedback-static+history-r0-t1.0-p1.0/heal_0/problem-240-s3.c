#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node* next;
} Node;

typedef struct List {
    Node* head;
    Node* tail;
} List;

Node* create_node(int data) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL) {
        return NULL;
    }
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}

List* create_list(void) {
    List* list = (List*)malloc(sizeof(List));
    if (list == NULL) {
        return NULL;
    }
    list->head = NULL;
    list->tail = NULL;
    return list;
}

int append_node(List* list, int data) {
    if (list == NULL) {
        return -1;
    }
    Node* new_node = create_node(data);
    if (new_node == NULL) {
        return -1;
    }
    if (list->tail == NULL) {
        list->head = new_node;
        list->tail = new_node;
    } else {
        list->tail->next = new_node;
        list->tail = new_node;
    }
    return 0;
}

void free_list(List* list) {
    if (list == NULL) {
        return;
    }
    Node* current = list->head;
    while (current != NULL) {
        Node* next = current->next;
        free(current);
        current = next;
    }
    free(list);
}

int replace_last_with_list(List* dest, List* src) {
    if (dest == NULL || src == NULL) {
        return -1;
    }
    if (dest->head == NULL) {
        dest->head = src->head;
        dest->tail = src->tail;
        src->head = NULL;
        src->tail = NULL;
        return 0;
    }
    if (dest->head == dest->tail) {
        Node* old_last = dest->head;
        dest->head = src->head;
        dest->tail = src->tail;
        free(old_last);
        src->head = NULL;
        src->tail = NULL;
        return 0;
    }
    Node* current = dest->head;
    while (current->next != dest->tail) {
        current = current->next;
    }
    free(dest->tail);
    current->next = src->head;
    dest->tail = src->tail;
    src->head = NULL;
    src->tail = NULL;
    return 0;
}

void print_list(List* list) {
    if (list == NULL) {
        return;
    }
    Node* current = list->head;
    while (current != NULL) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");
}

int main(void) {
    List* list1 = create_list();
    List* list2 = create_list();
    
    if (list1 == NULL || list2 == NULL) {
        fprintf(stderr, "Failed to allocate memory\n");
        free_list(list1);
        free_list(list2);
        return 1;
    }
    
    append_node(list1, 1);
    append_node(list1, 2);
    append_node(list1, 3);
    append_node(list1, 4);
    
    append_node(list2, 7);
    append_node(list2, 8);
    append_node(list2, 9);
    
    printf("List1 before replacement: ");
    print_list(list1);
    printf("List2: ");
    print_list(list2);
    
    if (replace_last_with_list(list1, list2) != 0) {
        fprintf(stderr, "Replacement failed\n");
        free_list(list1);
        free_list(list2);
        return 1;
    }
    
    printf("List1 after replacement: ");
    print_list(list1);
    
    free_list(list1);
    free_list(list2);
    
    return 0;
}