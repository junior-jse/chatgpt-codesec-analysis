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

int* get_last_elements(List* lists, int list_count, int* result_count) {
    /* Possible weaknesses found:
     *  Assuming that condition 'result_count==NULL' is not redundant
     */
    if (lists == NULL || list_count <= 0 || result_count == NULL) {
        /* Possible weaknesses found:
         *  Either the condition 'result_count==NULL' is redundant or there is possible null pointer dereference: result_count. [nullPointerRedundantCheck]
         *  Null pointer dereference
         */
        *result_count = 0;
        return NULL;
    }
    
    int* result = (int*)malloc(list_count * sizeof(int));
    if (result == NULL) {
        *result_count = 0;
        return NULL;
    }
    
    int count = 0;
    List* current_list = lists;
    
    while (current_list != NULL && count < list_count) {
        if (current_list->head == NULL) {
            free(result);
            *result_count = 0;
            return NULL;
        }
        
        Node* current_node = current_list->head;
        while (current_node->next != NULL) {
            current_node = current_node->next;
        }
        
        result[count] = current_node->data;
        count++;
        current_list = current_list->next;
    }
    
    *result_count = count;
    return result;
}

Node* create_node(int data) {
    Node* node = (Node*)malloc(sizeof(Node));
    if (node == NULL) {
        return NULL;
    }
    node->data = data;
    node->next = NULL;
    return node;
}

List* create_list(Node* head) {
    List* list = (List*)malloc(sizeof(List));
    if (list == NULL) {
        return NULL;
    }
    list->head = head;
    list->next = NULL;
    return list;
}

void free_list(List* list) {
    while (list != NULL) {
        List* next_list = list->next;
        Node* current = list->head;
        while (current != NULL) {
            Node* next_node = current->next;
            free(current);
            current = next_node;
        }
        free(list);
        list = next_list;
    }
}

int main() {
    Node* n1 = create_node(1);
    Node* n2 = create_node(2);
    Node* n3 = create_node(3);
    if (n1 != NULL) n1->next = n2;
    if (n2 != NULL) n2->next = n3;
    
    Node* n4 = create_node(4);
    Node* n5 = create_node(5);
    if (n4 != NULL) n4->next = n5;
    
    Node* n6 = create_node(6);
    
    List* l1 = create_list(n1);
    List* l2 = create_list(n4);
    List* l3 = create_list(n6);
    
    if (l1 != NULL) l1->next = l2;
    if (l2 != NULL) l2->next = l3;
    
    int result_count = 0;
    int* last_elements = get_last_elements(l1, 3, &result_count);
    
    if (last_elements != NULL) {
        for (int i = 0; i < result_count; i++) {
            printf("%d ", last_elements[i]);
        }
        printf("\n");
        free(last_elements);
    }
    
    free_list(l1);
    
    return 0;
}