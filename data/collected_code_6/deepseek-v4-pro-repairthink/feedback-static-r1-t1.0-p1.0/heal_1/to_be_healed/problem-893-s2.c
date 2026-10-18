#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node* next;
} Node;

typedef struct Sublist {
    Node* head;
    struct Sublist* next;
} Sublist;

int* get_last_elements(Sublist* sublists, int sublist_count, int* result_count) {
    /* Possible weaknesses found:
     *  Assuming that condition 'result_count==NULL' is not redundant
     */
    if (sublists == NULL || result_count == NULL) {
        /* Possible weaknesses found:
         *  Either the condition 'result_count==NULL' is redundant or there is possible null pointer dereference: result_count. [nullPointerRedundantCheck]
         *  Null pointer dereference
         */
        *result_count = 0;
        return NULL;
    }
    
    int* result = (int*)malloc(sizeof(int) * sublist_count);
    if (result == NULL) {
        *result_count = 0;
        return NULL;
    }
    
    Sublist* current_sublist = sublists;
    int index = 0;
    
    while (current_sublist != NULL && index < sublist_count) {
        Node* current_node = current_sublist->head;
        
        if (current_node == NULL) {
            free(result);
            *result_count = 0;
            return NULL;
        }
        
        while (current_node->next != NULL) {
            current_node = current_node->next;
        }
        
        result[index] = current_node->data;
        index++;
        current_sublist = current_sublist->next;
    }
    
    *result_count = index;
    return result;
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

Sublist* create_sublist(Node* head) {
    Sublist* new_sublist = (Sublist*)malloc(sizeof(Sublist));
    if (new_sublist == NULL) {
        return NULL;
    }
    new_sublist->head = head;
    new_sublist->next = NULL;
    return new_sublist;
}

void free_sublists(Sublist* sublists) {
    Sublist* current_sublist = sublists;
    while (current_sublist != NULL) {
        Sublist* next_sublist = current_sublist->next;
        Node* current_node = current_sublist->head;
        while (current_node != NULL) {
            Node* next_node = current_node->next;
            free(current_node);
            current_node = next_node;
        }
        free(current_sublist);
        current_sublist = next_sublist;
    }
}

int main() {
    Node* list1_node1 = create_node(1);
    Node* list1_node2 = create_node(2);
    Node* list1_node3 = create_node(3);
    list1_node1->next = list1_node2;
    list1_node2->next = list1_node3;
    
    Node* list2_node1 = create_node(4);
    Node* list2_node2 = create_node(5);
    list2_node1->next = list2_node2;
    
    Node* list3_node1 = create_node(6);
    
    Sublist* sublist1 = create_sublist(list1_node1);
    Sublist* sublist2 = create_sublist(list2_node1);
    Sublist* sublist3 = create_sublist(list3_node1);
    
    sublist1->next = sublist2;
    sublist2->next = sublist3;
    
    int result_count = 0;
    int* last_elements = get_last_elements(sublist1, 3, &result_count);
    
    if (last_elements != NULL) {
        for (int i = 0; i < result_count; i++) {
            printf("%d ", last_elements[i]);
        }
        printf("\n");
        free(last_elements);
    }
    
    free_sublists(sublist1);
    
    return 0;
}