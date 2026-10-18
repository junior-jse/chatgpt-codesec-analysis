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

int is_empty_list(const List* list) {
    return list == NULL || list->head == NULL;
}

void free_list_nodes(Node* head) {
    Node* current = head;
    while (current != NULL) {
        Node* temp = current;
        current = current->next;
        free(temp);
    }
}

void free_single_list(List* list) {
    if (list == NULL) return;
    free_list_nodes(list->head);
    free(list);
}

List* remove_empty_lists(List* head) {
    if (head == NULL) return NULL;

    List* dummy = (List*)malloc(sizeof(List));
    if (dummy == NULL) return NULL;
    
    dummy->head = NULL;
    dummy->next = head;
    
    List* prev = dummy;
    List* current = head;
    
    while (current != NULL) {
        List* next = current->next;
        
        if (is_empty_list(current)) {
            prev->next = next;
            free_single_list(current);
        } else {
            prev = current;
        }
        
        current = next;
    }
    
    List* result = dummy->next;
    free(dummy);
    return result;
}

List* create_list_node(Node* head) {
    List* list = (List*)malloc(sizeof(List));
    if (list == NULL) return NULL;
    list->head = head;
    list->next = NULL;
    return list;
}

Node* create_node(int data) {
    Node* node = (Node*)malloc(sizeof(Node));
    if (node == NULL) return NULL;
    node->data = data;
    node->next = NULL;
    return node;
}

void append_node(Node** head, int data) {
    Node* new_node = create_node(data);
    if (new_node == NULL) return;
    
    if (*head == NULL) {
        *head = new_node;
        return;
    }
    
    Node* current = *head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = new_node;
}

void print_lists(List* head) {
    List* current = head;
    int list_index = 0;
    
    while (current != NULL) {
        printf("List %d: ", list_index);
        Node* node = current->head;
        
        if (node == NULL) {
            printf("(empty)");
        } else {
            while (node != NULL) {
                printf("%d", node->data);
                if (node->next != NULL) printf(" -> ");
                node = node->next;
            }
        }
        printf("\n");
        current = current->next;
        list_index++;
    }
}

void free_all_lists(List* head) {
    List* current = head;
    while (current != NULL) {
        List* temp = current;
        current = current->next;
        free_single_list(temp);
    }
}

int main() {
    List* head = NULL;
    List* tail = NULL;
    
    List* list1 = create_list_node(NULL);
    List* list2 = create_list_node(NULL);
    List* list3 = create_list_node(NULL);
    List* list4 = create_list_node(NULL);
    
    if (list1 == NULL || list2 == NULL || list3 == NULL || list4 == NULL) {
        return 1;
    }
    
    append_node(&(list2->head), 10);
    append_node(&(list2->head), 20);
    append_node(&(list2->head), 30);
    
    append_node(&(list4->head), 5);
    
    head = list1;
    tail = list1;
    tail->next = list2;
    tail = list2;
    tail->next = list3;
    tail = list3;
    tail->next = list4;
    
    printf("Original lists:\n");
    print_lists(head);
    printf("\n");
    
    head = remove_empty_lists(head);
    
    printf("Lists after removing empty ones:\n");
    print_lists(head);
    
    free_all_lists(head);
    
    return 0;
}