#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    void *data;
    struct Node *next;
} Node;

typedef struct List {
    Node *head;
} List;

int count_lists_and_square(List *list_of_lists) {
    int count = 0;
    Node *current = list_of_lists->head;
    
    while (current != NULL) {
        count++;
        current = current->next;
    }
    
    return count * count;
}

List *create_list(void) {
    List *list = (List *)malloc(sizeof(List));
    if (list == NULL) {
        return NULL;
    }
    list->head = NULL;
    return list;
}

int add_node(List *list, void *data) {
    Node *new_node = (Node *)malloc(sizeof(Node));
    if (new_node == NULL) {
        return 0;
    }
    new_node->data = data;
    new_node->next = list->head;
    list->head = new_node;
    return 1;
}

void free_list(List *list, int free_data) {
    Node *current = list->head;
    while (current != NULL) {
        Node *temp = current;
        current = current->next;
        if (free_data) {
            free(temp->data);
        }
        free(temp);
    }
    free(list);
}

int main(void) {
    List *outer_list = create_list();
    if (outer_list == NULL) {
        return 1;
    }
    
    List *inner_list1 = create_list();
    List *inner_list2 = create_list();
    List *inner_list3 = create_list();
    
    if (inner_list1 == NULL || inner_list2 == NULL || inner_list3 == NULL) {
        if (inner_list1 != NULL) free_list(inner_list1, 0);
        if (inner_list2 != NULL) free_list(inner_list2, 0);
        if (inner_list3 != NULL) free_list(inner_list3, 0);
        free_list(outer_list, 0);
        return 1;
    }
    
    if (!add_node(outer_list, inner_list1) || 
        !add_node(outer_list, inner_list2) || 
        !add_node(outer_list, inner_list3)) {
        free_list(inner_list1, 0);
        free_list(inner_list2, 0);
        free_list(inner_list3, 0);
        free_list(outer_list, 0);
        return 1;
    }
    
    int result = count_lists_and_square(outer_list);
    printf("%d\n", result);
    
    Node *current = outer_list->head;
    while (current != NULL) {
        List *inner = (List *)current->data;
        free_list(inner, 0);
        current = current->next;
    }
    free_list(outer_list, 0);
    
    return 0;
}