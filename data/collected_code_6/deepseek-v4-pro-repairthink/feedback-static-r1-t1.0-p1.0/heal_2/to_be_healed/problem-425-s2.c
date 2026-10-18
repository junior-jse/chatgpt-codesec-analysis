#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node* next;
} Node;

typedef struct {
    Node* head;
    int size;
} LinkedList;

typedef struct {
    LinkedList** lists;
    int count;
} ListOfLists;

Node* create_node(int data) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL) {
        return NULL;
    }
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}

LinkedList* create_list(void) {
    LinkedList* list = (LinkedList*)malloc(sizeof(LinkedList));
    if (list == NULL) {
        return NULL;
    }
    list->head = NULL;
    list->size = 0;
    return list;
}

int insert_at_end(LinkedList* list, int data) {
    if (list == NULL) {
        return -1;
    }
    
    Node* new_node = create_node(data);
    if (new_node == NULL) {
        return -1;
    }
    
    if (list->head == NULL) {
        list->head = new_node;
    } else {
        Node* current = list->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
    }
    
    list->size++;
    return 0;
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

void free_list_of_lists(ListOfLists* list_of_lists) {
    if (list_of_lists == NULL) {
        return;
    }
    
    for (int i = 0; i < list_of_lists->count; i++) {
        free_list(list_of_lists->lists[i]);
    }
    
    free(list_of_lists->lists);
    free(list_of_lists);
}

int count_sublists_with_element(LinkedList** lists, int num_lists, int element) {
    if (lists == NULL || num_lists <= 0) {
        return 0;
    }
    
    int count = 0;
    
    for (int i = 0; i < num_lists; i++) {
        if (lists[i] == NULL) {
            continue;
        }
        
        Node* current = lists[i]->head;
        while (current != NULL) {
            if (current->data == element) {
                count++;
                break;
            }
            current = current->next;
        }
    }
    
    return count;
}

int main(void) {
    ListOfLists* list_of_lists = (ListOfLists*)malloc(sizeof(ListOfLists));
    if (list_of_lists == NULL) {
        return 1;
    }
    
    list_of_lists->count = 3;
    list_of_lists->lists = (LinkedList**)malloc(list_of_lists->count * sizeof(LinkedList*));
    if (list_of_lists->lists == NULL) {
        free(list_of_lists);
        return 1;
    }
    
    for (int i = 0; i < list_of_lists->count; i++) {
        list_of_lists->lists[i] = create_list();
        if (list_of_lists->lists[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free_list(list_of_lists->lists[j]);
            }
            free(list_of_lists->lists);
            free(list_of_lists);
            return 1;
        }
    }
    
    /* Possible weaknesses found:
     *  Variable 'elements1' can be declared as const array [constVariable]
     */
    int elements1[] = {1, 2, 3, 4, 5};
    /* Possible weaknesses found:
     *  Variable 'elements2' can be declared as const array [constVariable]
     */
    int elements2[] = {6, 7, 8, 3, 9};
    /* Possible weaknesses found:
     *  Variable 'elements3' can be declared as const array [constVariable]
     */
    int elements3[] = {10, 11, 12};
    
    for (int i = 0; i < 5; i++) {
        if (insert_at_end(list_of_lists->lists[0], elements1[i]) != 0) {
            free_list_of_lists(list_of_lists);
            return 1;
        }
    }
    
    for (int i = 0; i < 5; i++) {
        if (insert_at_end(list_of_lists->lists[1], elements2[i]) != 0) {
            free_list_of_lists(list_of_lists);
            return 1;
        }
    }
    
    for (int i = 0; i < 3; i++) {
        if (insert_at_end(list_of_lists->lists[2], elements3[i]) != 0) {
            free_list_of_lists(list_of_lists);
            return 1;
        }
    }
    
    int result = count_sublists_with_element(list_of_lists->lists, list_of_lists->count, 3);
    printf("Number of sublists containing element 3: %d\n", result);
    
    result = count_sublists_with_element(list_of_lists->lists, list_of_lists->count, 100);
    printf("Number of sublists containing element 100: %d\n", result);
    
    result = count_sublists_with_element(list_of_lists->lists, 0, 3);
    printf("Number of sublists containing element 3 (invalid count): %d\n", result);
    
    result = count_sublists_with_element(NULL, 3, 3);
    printf("Number of sublists containing element 3 (NULL lists): %d\n", result);
    
    free_list_of_lists(list_of_lists);
    
    return 0;
}