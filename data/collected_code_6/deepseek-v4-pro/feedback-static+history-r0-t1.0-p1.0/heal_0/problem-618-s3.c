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

LinkedList* create_list() {
    LinkedList* list = (LinkedList*)malloc(sizeof(LinkedList));
    if (list == NULL) {
        return NULL;
    }
    list->head = NULL;
    list->size = 0;
    return list;
}

void destroy_list(LinkedList* list) {
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

int append_node(LinkedList* list, int value) {
    if (list == NULL) {
        return 0;
    }
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL) {
        return 0;
    }
    new_node->data = value;
    new_node->next = NULL;
    
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
    return 1;
}

int get_element(const LinkedList* list, int index) {
    if (list == NULL || index < 0 || index >= list->size) {
        return 0;
    }
    Node* current = list->head;
    for (int i = 0; i < index; i++) {
        current = current->next;
    }
    return current->data;
}

void print_list(const LinkedList* list) {
    if (list == NULL) {
        printf("NULL\n");
        return;
    }
    Node* current = list->head;
    if (current == NULL) {
        printf("[]\n");
        return;
    }
    printf("[");
    while (current != NULL) {
        printf("%d", current->data);
        if (current->next != NULL) {
            printf(", ");
        }
        current = current->next;
    }
    printf("]\n");
}

LinkedList* divide_lists(const LinkedList* numerator, const LinkedList* denominator) {
    if (numerator == NULL || denominator == NULL) {
        return NULL;
    }
    if (numerator->size != denominator->size) {
        return NULL;
    }
    if (numerator->size == 0) {
        return NULL;
    }
    
    for (int i = 0; i < denominator->size; i++) {
        if (get_element(denominator, i) == 0) {
            return NULL;
        }
    }
    
    LinkedList* result = create_list();
    if (result == NULL) {
        return NULL;
    }
    
    for (int i = 0; i < numerator->size; i++) {
        int quotient = get_element(numerator, i) / get_element(denominator, i);
        if (!append_node(result, quotient)) {
            destroy_list(result);
            return NULL;
        }
    }
    
    return result;
}

int main() {
    LinkedList* list1 = create_list();
    LinkedList* list2 = create_list();
    
    if (list1 == NULL || list2 == NULL) {
        destroy_list(list1);
        destroy_list(list2);
        return 1;
    }
    
    append_node(list1, 10);
    append_node(list1, 20);
    append_node(list1, 30);
    
    append_node(list2, 2);
    append_node(list2, 5);
    append_node(list2, 10);
    
    printf("List 1: ");
    print_list(list1);
    printf("List 2: ");
    print_list(list2);
    
    LinkedList* result = divide_lists(list1, list2);
    
    if (result == NULL) {
        printf("Division failed\n");
    } else {
        printf("Result: ");
        print_list(result);
        destroy_list(result);
    }
    
    destroy_list(list1);
    destroy_list(list2);
    
    return 0;
}