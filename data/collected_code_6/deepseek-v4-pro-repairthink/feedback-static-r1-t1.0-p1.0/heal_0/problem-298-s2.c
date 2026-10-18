#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Node {
    int data;
    struct Node* next;
    struct Node* child;
} Node;

Node* create_node(int data) {
    Node* node = (Node*)malloc(sizeof(Node));
    if (node == NULL) {
        return NULL;
    }
    node->data = data;
    node->next = NULL;
    node->child = NULL;
    return node;
}

bool contains(Node* list, int value) {
    Node* current = list;
    while (current != NULL) {
        if (current->data == value) {
            return true;
        }
        current = current->next;
    }
    return false;
}

void collect_elements(Node* nested_list, Node* flat_list, Node** result) {
    if (nested_list == NULL) {
        return;
    }
    
    Node* current = nested_list;
    while (current != NULL) {
        if (current->child != NULL) {
            collect_elements(current->child, flat_list, result);
        } else {
            if (contains(flat_list, current->data)) {
                Node* new_node = create_node(current->data);
                if (new_node != NULL) {
                    new_node->next = *result;
                    *result = new_node;
                }
            }
        }
        current = current->next;
    }
}

Node* find_common_elements(Node* nested_list, Node* flat_list) {
    Node* result = NULL;
    collect_elements(nested_list, flat_list, &result);
    return result;
}

void free_list(Node* list) {
    Node* current = list;
    while (current != NULL) {
        Node* next = current->next;
        free(current);
        current = next;
    }
}

void free_nested_list(Node* list) {
    if (list == NULL) {
        return;
    }
    
    Node* current = list;
    while (current != NULL) {
        Node* next = current->next;
        if (current->child != NULL) {
            free_nested_list(current->child);
        }
        free(current);
        current = next;
    }
}

void print_list(Node* list) {
    Node* current = list;
    while (current != NULL) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");
}

int main() {
    Node* flat_list = create_node(1);
    flat_list->next = create_node(2);
    flat_list->next->next = create_node(3);
    flat_list->next->next->next = create_node(4);
    flat_list->next->next->next->next = create_node(5);
    
    Node* nested_list = create_node(10);
    nested_list->next = create_node(2);
    nested_list->next->next = create_node(20);
    nested_list->next->next->next = create_node(30);
    
    nested_list->next->child = create_node(4);
    nested_list->next->child->next = create_node(6);
    nested_list->next->child->next->next = create_node(8);
    
    nested_list->next->next->child = create_node(3);
    nested_list->next->next->child->next = create_node(9);
    
    Node* common = find_common_elements(nested_list, flat_list);
    
    printf("Common elements: ");
    print_list(common);
    
    free_list(common);
    free_nested_list(nested_list);
    free_list(flat_list);
    
    return 0;
}