#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node* next;
} Node;

typedef struct Sublist {
    Node* elements;
    struct Sublist* next;
} Sublist;

int get_last_element(Node* head, int* result) {
    if (head == NULL || result == NULL) {
        return 0;
    }
    
    Node* current = head;
    while (current->next != NULL) {
        current = current->next;
    }
    
    *result = current->data;
    return 1;
}

int* get_last_elements(Sublist* sublist_head, int* count) {
    if (sublist_head == NULL || count == NULL) {
        return NULL;
    }
    
    int num_sublists = 0;
    Sublist* current_sublist = sublist_head;
    while (current_sublist != NULL) {
        num_sublists++;
        current_sublist = current_sublist->next;
    }
    
    int* results = (int*)malloc(num_sublists * sizeof(int));
    if (results == NULL) {
        *count = 0;
        return NULL;
    }
    
    current_sublist = sublist_head;
    int index = 0;
    while (current_sublist != NULL) {
        int last_element;
        if (!get_last_element(current_sublist->elements, &last_element)) {
            free(results);
            *count = 0;
            return NULL;
        }
        results[index++] = last_element;
        current_sublist = current_sublist->next;
    }
    
    *count = num_sublists;
    return results;
}

Node* create_node(int data) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL) {
        return NULL;
    }
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}

Sublist* create_sublist(Node* elements) {
    Sublist* new_sublist = (Sublist*)malloc(sizeof(Sublist));
    if (new_sublist == NULL) {
        return NULL;
    }
    new_sublist->elements = elements;
    new_sublist->next = NULL;
    return new_sublist;
}

void free_nodes(Node* head) {
    Node* current = head;
    while (current != NULL) {
        Node* temp = current;
        current = current->next;
        free(temp);
    }
}

void free_sublists(Sublist* head) {
    Sublist* current = head;
    while (current != NULL) {
        Sublist* temp = current;
        free_nodes(current->elements);
        current = current->next;
        free(temp);
    }
}

int main() {
    Node* list1_node1 = create_node(1);
    Node* list1_node2 = create_node(2);
    Node* list1_node3 = create_node(3);
    if (list1_node1 == NULL || list1_node2 == NULL || list1_node3 == NULL) {
        free_nodes(list1_node1);
        free_nodes(list1_node2);
        free_nodes(list1_node3);
        return 1;
    }
    list1_node1->next = list1_node2;
    list1_node2->next = list1_node3;
    
    Node* list2_node1 = create_node(4);
    Node* list2_node2 = create_node(5);
    if (list2_node1 == NULL || list2_node2 == NULL) {
        free_nodes(list1_node1);
        free_nodes(list2_node1);
        free_nodes(list2_node2);
        return 1;
    }
    list2_node1->next = list2_node2;
    
    Node* list3_node1 = create_node(6);
    if (list3_node1 == NULL) {
        free_nodes(list1_node1);
        free_nodes(list2_node1);
        return 1;
    }
    
    Sublist* sublist1 = create_sublist(list1_node1);
    Sublist* sublist2 = create_sublist(list2_node1);
    Sublist* sublist3 = create_sublist(list3_node1);
    
    if (sublist1 == NULL || sublist2 == NULL || sublist3 == NULL) {
        free_sublists(sublist1);
        free_sublists(sublist2);
        free_sublists(sublist3);
        return 1;
    }
    
    sublist1->next = sublist2;
    sublist2->next = sublist3;
    
    int count = 0;
    int* last_elements = get_last_elements(sublist1, &count);
    
    if (last_elements != NULL) {
        for (int i = 0; i < count; i++) {
            printf("%d ", last_elements[i]);
        }
        printf("\n");
        free(last_elements);
    }
    
    free_sublists(sublist1);
    
    return 0;
}