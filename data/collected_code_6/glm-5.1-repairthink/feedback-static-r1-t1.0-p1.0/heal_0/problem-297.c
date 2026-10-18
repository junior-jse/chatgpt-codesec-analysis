#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Node {
    bool is_list;
    union {
        int value;
        struct {
            struct Node* head;
            struct Node* tail;
        } list;
    } data;
} Node;

typedef struct IntNode {
    int value;
    struct IntNode* next;
} IntNode;

Node* create_int_node(int val) {
    Node* n = (Node*)malloc(sizeof(Node));
    if (!n) return NULL;
    n->is_list = false;
    n->data.value = val;
    return n;
}

Node* create_list_node(Node* head, Node* tail) {
    Node* n = (Node*)malloc(sizeof(Node));
    if (!n) return NULL;
    n->is_list = true;
    n->data.list.head = head;
    n->data.list.tail = tail;
    return n;
}

IntNode* create_flattened_node(int val, IntNode* next) {
    IntNode* n = (IntNode*)malloc(sizeof(IntNode));
    if (!n) return NULL;
    n->value = val;
    n->next = next;
    return n;
}

int flatten_helper(Node* node, IntNode** current) {
    if (!node) return 0;
    
    if (!node->is_list) {
        IntNode* new_node = create_flattened_node(node->data.value, NULL);
        if (!new_node) return -1;
        
        if (*current) {
            (*current)->next = new_node;
        }
        *current = new_node;
        return 1;
    }
    
    int count = 0;
    Node* child = node->data.list.head;
    
    while (child) {
        int res = flatten_helper(child, current);
        if (res < 0) return -1;
        count += res;
        child = child->data.list.tail;
    }
    
    return count;
}

int flatten(Node* root, IntNode** out_head) {
    if (!root || !out_head) return -1;
    
    IntNode* head = NULL;
    IntNode* current = NULL;
    
    int count = flatten_helper(root, &current);
    
    if (count < 0) {
        IntNode* temp = current;
        while (temp) {
            IntNode* next = temp->next;
            free(temp);
            temp = next;
        }
        *out_head = NULL;
        return -1;
    }
    
    head = current;
    while (head && head->next) {
        head = head->next;
    }

    IntNode* prev = NULL;
    IntNode* curr = current;
    while (curr) {
        IntNode* next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }
    
    *out_head = prev;
    return count;
}

void free_nested_list(Node* node) {
    if (!node) return;
    if (node->is_list) {
        Node* child = node->data.list.head;
        while (child) {
            Node* next = child->data.list.tail;
            free_nested_list(child);
            child = next;
        }
    }
    free(node);
}

void free_flat_list(IntNode* head) {
    IntNode* current = head;
    while (current) {
        IntNode* next = current->next;
        free(current);
        current = next;
    }
}

int main(void) {
    Node* n1 = create_int_node(1);
    Node* n2 = create_int_node(2);
    Node* n3 = create_int_node(3);
    Node* n4 = create_int_node(4);
    Node* n5 = create_int_node(5);
    
    if (!n1 || !n2 || !n3 || !n4 || !n5) {
        free_nested_list(n1);
        free_nested_list(n2);
        free_nested_list(n3);
        free_nested_list(n4);
        free_nested_list(n5);
        return 1;
    }

    Node* list1 = create_list_node(n3, NULL);
    Node* inner_list = create_list_node(n2, list1);
    Node* outer_list = create_list_node(n1, inner_list);
    
    Node* list2 = create_list_node(n4, NULL);
    Node* list3 = create_list_node(n5, NULL);
    Node* combined = create_list_node(list2, list3);
    Node* root = create_list_node(outer_list, combined);

    if (!list1 || !inner_list || !outer_list || !list2 || !list3 || !combined || !root) {
        free_nested_list(root);
        return 1;
    }

    IntNode* flat_head = NULL;
    int count = flatten(root, &flat_head);
    
    if (count < 0) {
        printf("Flattening failed\n");
        free_nested_list(root);
        return 1;
    }
    
    printf("Flattened list (%d items): ", count);
    IntNode* curr = flat_head;
    while (curr) {
        printf("%d ", curr->value);
        curr = curr->next;
    }
    printf("\n");
    
    free_flat_list(flat_head);
    free_nested_list(root);
    
    return 0;
}