#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    int data;
    struct Node* next;
} Node;

Node* create_node(int data) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL) {
        return NULL;
    }
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}

void append_node(Node** head, Node** tail, int data) {
    Node* new_node = create_node(data);
    if (new_node == NULL) {
        return;
    }
    if (*head == NULL) {
        *head = new_node;
        *tail = new_node;
    } else {
        (*tail)->next = new_node;
        *tail = new_node;
    }
}

Node* interleave_lists(Node* list1, Node* list2) {
    if (list1 == NULL && list2 == NULL) {
        return NULL;
    }
    
    Node* result_head = NULL;
    Node* result_tail = NULL;
    Node* current1 = list1;
    Node* current2 = list2;
    
    while (current1 != NULL || current2 != NULL) {
        if (current1 != NULL) {
            Node* new_node = create_node(current1->data);
            if (new_node == NULL) {
                while (result_head != NULL) {
                    Node* temp = result_head;
                    result_head = result_head->next;
                    free(temp);
                }
                return NULL;
            }
            if (result_head == NULL) {
                result_head = new_node;
                result_tail = new_node;
            } else {
                result_tail->next = new_node;
                result_tail = new_node;
            }
            current1 = current1->next;
        }
        
        if (current2 != NULL) {
            Node* new_node = create_node(current2->data);
            if (new_node == NULL) {
                while (result_head != NULL) {
                    Node* temp = result_head;
                    result_head = result_head->next;
                    free(temp);
                }
                return NULL;
            }
            result_tail->next = new_node;
            result_tail = new_node;
            current2 = current2->next;
        }
    }
    
    return result_head;
}

void print_list(Node* head) {
    Node* current = head;
    while (current != NULL) {
        printf("%d", current->data);
        if (current->next != NULL) {
            printf(" -> ");
        }
        current = current->next;
    }
    printf("\n");
}

void free_list(Node* head) {
    Node* current = head;
    while (current != NULL) {
        Node* temp = current;
        current = current->next;
        free(temp);
    }
}

int main(void) {
    Node* list1 = NULL;
    Node* list1_tail = NULL;
    Node* list2 = NULL;
    Node* list2_tail = NULL;
    
    append_node(&list1, &list1_tail, 1);
    append_node(&list1, &list1_tail, 3);
    append_node(&list1, &list1_tail, 5);
    
    append_node(&list2, &list2_tail, 2);
    append_node(&list2, &list2_tail, 4);
    append_node(&list2, &list2_tail, 6);
    
    printf("List 1: ");
    print_list(list1);
    printf("List 2: ");
    print_list(list2);
    
    Node* interleaved = interleave_lists(list1, list2);
    
    if (interleaved != NULL) {
        printf("Interleaved: ");
        print_list(interleaved);
        free_list(interleaved);
    } else {
        printf("Failed to create interleaved list\n");
    }
    
    free_list(list1);
    free_list(list2);
    
    return 0;
}