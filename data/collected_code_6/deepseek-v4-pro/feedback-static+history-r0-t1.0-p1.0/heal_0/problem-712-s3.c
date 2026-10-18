#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

typedef struct List {
    Node *head;
    struct List *next;
} List;

List* create_list() {
    List *list = (List*)malloc(sizeof(List));
    if (!list) return NULL;
    list->head = NULL;
    list->next = NULL;
    return list;
}

Node* create_node(int data) {
    Node *node = (Node*)malloc(sizeof(Node));
    if (!node) return NULL;
    node->data = data;
    node->next = NULL;
    return node;
}

int lists_equal(Node *a, Node *b) {
    Node *pa = a, *pb = b;
    while (pa && pb) {
        if (pa->data != pb->data) return 0;
        pa = pa->next;
        pb = pb->next;
    }
    return pa == NULL && pb == NULL;
}

int list_exists(List *head, Node *candidate) {
    List *current = head;
    while (current) {
        if (lists_equal(current->head, candidate)) return 1;
        current = current->next;
    }
    return 0;
}

void free_node_list(Node *head) {
    Node *current = head;
    while (current) {
        Node *temp = current;
        current = current->next;
        free(temp);
    }
}

void free_list_list(List *head) {
    List *current = head;
    while (current) {
        List *temp = current;
        free_node_list(current->head);
        current = current->next;
        free(temp);
    }
}

List* append_to_list(List *head, Node *node_list) {
    List *new_list = create_list();
    if (!new_list) return head;
    new_list->head = node_list;
    new_list->next = NULL;
    
    if (!head) return new_list;
    
    List *current = head;
    while (current->next) current = current->next;
    current->next = new_list;
    return head;
}

List* remove_duplicates(List *head) {
    if (!head) return NULL;
    
    List *result = NULL;
    List *current = head;
    
    while (current) {
        if (!list_exists(result, current->head)) {
            Node *copy_head = NULL;
            Node *copy_tail = NULL;
            Node *src = current->head;
            
            while (src) {
                Node *new_node = create_node(src->data);
                if (!new_node) {
                    free_node_list(copy_head);
                    free_list_list(result);
                    return NULL;
                }
                if (!copy_head) {
                    copy_head = new_node;
                    copy_tail = new_node;
                } else {
                    copy_tail->next = new_node;
                    copy_tail = new_node;
                }
                src = src->next;
            }
            
            result = append_to_list(result, copy_head);
        }
        current = current->next;
    }
    
    return result;
}

int main() {
    List *ll = create_list();
    
    Node *n1 = create_node(1);
    n1->next = create_node(2);
    n1->next->next = create_node(3);
    ll = append_to_list(ll, n1);
    
    Node *n2 = create_node(4);
    n2->next = create_node(5);
    ll = append_to_list(ll, n2);
    
    Node *n3 = create_node(1);
    n3->next = create_node(2);
    n3->next->next = create_node(3);
    ll = append_to_list(ll, n3);
    
    Node *n4 = create_node(6);
    ll = append_to_list(ll, n4);
    
    List *result = remove_duplicates(ll);
    
    List *iter = result;
    while (iter) {
        Node *p = iter->head;
        while (p) {
            printf("%d ", p->data);
            p = p->next;
        }
        printf("\n");
        iter = iter->next;
    }
    
    free_list_list(ll);
    free_list_list(result);
    
    return 0;
}