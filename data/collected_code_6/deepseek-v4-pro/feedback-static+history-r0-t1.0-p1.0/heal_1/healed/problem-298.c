#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Node {
    int data;
    struct Node* next;
    struct Node* child;
} Node;

Node* create_node(int data) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    new_node->data = data;
    new_node->next = NULL;
    new_node->child = NULL;
    return new_node;
}

bool contains_element(Node* list, int value) {
    Node* current = list;
    while (current != NULL) {
        if (current->data == value) {
            return true;
        }
        current = current->next;
    }
    return false;
}

void find_nested_elements(Node* nested_list, Node* main_list, Node** result_head, Node** result_tail) {
    if (nested_list == NULL) {
        return;
    }
    
    Node* current = nested_list;
    
    while (current != NULL) {
        if (contains_element(main_list, current->data)) {
            Node* new_node = create_node(current->data);
            if (*result_head == NULL) {
                *result_head = new_node;
                *result_tail = new_node;
            } else {
                (*result_tail)->next = new_node;
                *result_tail = new_node;
            }
        }
        
        if (current->child != NULL) {
            Node* child_head = NULL;
            Node* child_tail = NULL;
            find_nested_elements(current->child, main_list, &child_head, &child_tail);
            if (child_head != NULL) {
                if (*result_head == NULL) {
                    *result_head = child_head;
                    *result_tail = child_tail;
                } else {
                    (*result_tail)->next = child_head;
                    *result_tail = child_tail;
                }
            }
        }
        
        current = current->next;
    }
}

void free_list(Node* head) {
    Node* current = head;
    while (current != NULL) {
        Node* next = current->next;
        if (current->child != NULL) {
            free_list(current->child);
        }
        free(current);
        current = next;
    }
}

void print_list(Node* head) {
    Node* current = head;
    while (current != NULL) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");
}

Node* build_sample_nested_list() {
    Node* n1 = create_node(1);
    Node* n2 = create_node(2);
    Node* n3 = create_node(3);
    Node* n4 = create_node(4);
    Node* n5 = create_node(5);
    Node* n6 = create_node(6);
    Node* n7 = create_node(7);
    Node* n8 = create_node(8);
    
    n1->next = n2;
    n1->child = n4;
    n2->next = n3;
    n2->child = n5;
    n4->next = n5;
    n4->child = n7;
    n5->next = n6;
    n5->child = n8;
    n6->next = NULL;
    n7->next = NULL;
    n8->next = NULL;
    
    return n1;
}

Node* build_sample_main_list() {
    Node* n1 = create_node(2);
    Node* n2 = create_node(3);
    Node* n3 = create_node(5);
    Node* n4 = create_node(8);
    Node* n5 = create_node(10);
    
    n1->next = n2;
    n2->next = n3;
    n3->next = n4;
    n4->next = n5;
    n5->next = NULL;
    
    return n1;
}

int main() {
    Node* nested_list = build_sample_nested_list();
    Node* main_list = build_sample_main_list();
    Node* result_head = NULL;
    Node* result_tail = NULL;
    
    find_nested_elements(nested_list, main_list, &result_head, &result_tail);
    
    printf("Nested elements found in main list: ");
    print_list(result_head);
    
    free_list(nested_list);
    free_list(main_list);
    free_list(result_head);
    
    return 0;
}