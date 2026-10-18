#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node *next;
};

struct SubList {
    struct Node *head;
    struct SubList *next;
};

int *get_last_elements(struct SubList *lists, int *count) {
    int length = 0;
    struct SubList *current = lists;
    while (current != NULL) {
        length++;
        current = current->next;
    }
    
    if (length == 0) {
        *count = 0;
        return NULL;
    }
    
    int *result = (int *)malloc(length * sizeof(int));
    if (result == NULL) {
        *count = 0;
        return NULL;
    }
    
    current = lists;
    for (int i = 0; i < length; i++) {
        if (current->head == NULL) {
            free(result);
            *count = 0;
            return NULL;
        }
        
        struct Node *inner = current->head;
        while (inner->next != NULL) {
            inner = inner->next;
        }
        result[i] = inner->data;
        current = current->next;
    }
    
    *count = length;
    return result;
}

void free_sublists(struct SubList *lists) {
    struct SubList *current_list = lists;
    while (current_list != NULL) {
        struct Node *current_node = current_list->head;
        while (current_node != NULL) {
            struct Node *next_node = current_node->next;
            free(current_node);
            current_node = next_node;
        }
        struct SubList *next_list = current_list->next;
        free(current_list);
        current_list = next_list;
    }
}

int main(void) {
    struct Node *n1 = (struct Node *)malloc(sizeof(struct Node));
    struct Node *n2 = (struct Node *)malloc(sizeof(struct Node));
    struct Node *n3 = (struct Node *)malloc(sizeof(struct Node));
    struct Node *n4 = (struct Node *)malloc(sizeof(struct Node));
    struct Node *n5 = (struct Node *)malloc(sizeof(struct Node));
    
    n1->data = 1; n1->next = n2;
    n2->data = 2; n2->next = NULL;
    
    n3->data = 3; n3->next = n4;
    n4->data = 4; n4->next = n5;
    n5->data = 5; n5->next = NULL;
    
    struct SubList *l1 = (struct SubList *)malloc(sizeof(struct SubList));
    struct SubList *l2 = (struct SubList *)malloc(sizeof(struct SubList));
    
    l1->head = n1; l1->next = l2;
    l2->head = n3; l2->next = NULL;
    
    int count = 0;
    int *last_elements = get_last_elements(l1, &count);
    
    if (last_elements != NULL) {
        for (int i = 0; i < count; i++) {
            printf("%d\n", last_elements[i]);
        }
        free(last_elements);
    }
    
    free_sublists(l1);
    
    return 0;
}