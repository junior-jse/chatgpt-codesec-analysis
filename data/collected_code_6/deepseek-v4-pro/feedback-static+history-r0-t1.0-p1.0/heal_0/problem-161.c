#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

typedef struct List {
    Node *head;
    size_t size;
} List;

List *list_create(void) {
    List *list = (List *)malloc(sizeof(List));
    if (list == NULL) {
        return NULL;
    }
    list->head = NULL;
    list->size = 0;
    return list;
}

bool list_push_back(List *list, int value) {
    if (list == NULL) {
        return false;
    }
    Node *new_node = (Node *)malloc(sizeof(Node));
    if (new_node == NULL) {
        return false;
    }
    new_node->data = value;
    new_node->next = NULL;
    
    if (list->head == NULL) {
        list->head = new_node;
    } else {
        Node *current = list->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
    }
    list->size++;
    return true;
}

bool list_contains(const List *list, int value) {
    if (list == NULL) {
        return false;
    }
    Node *current = list->head;
    while (current != NULL) {
        if (current->data == value) {
            return true;
        }
        current = current->next;
    }
    return false;
}

void list_remove_all_present_in_other(List *list1, const List *list2) {
    if (list1 == NULL || list2 == NULL) {
        return;
    }
    
    Node *current = list1->head;
    Node *prev = NULL;
    
    while (current != NULL) {
        if (list_contains(list2, current->data)) {
            Node *to_delete = current;
            if (prev == NULL) {
                list1->head = current->next;
                current = list1->head;
            } else {
                prev->next = current->next;
                current = current->next;
            }
            free(to_delete);
            list1->size--;
        } else {
            prev = current;
            current = current->next;
        }
    }
}

void list_destroy(List *list) {
    if (list == NULL) {
        return;
    }
    Node *current = list->head;
    while (current != NULL) {
        Node *next = current->next;
        free(current);
        current = next;
    }
    free(list);
}

void list_print(const List *list) {
    if (list == NULL) {
        printf("NULL\n");
        return;
    }
    Node *current = list->head;
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

int main(void) {
    List *list1 = list_create();
    List *list2 = list_create();
    
    if (list1 == NULL || list2 == NULL) {
        if (list1 != NULL) list_destroy(list1);
        if (list2 != NULL) list_destroy(list2);
        return 1;
    }
    
    list_push_back(list1, 1);
    list_push_back(list1, 2);
    list_push_back(list1, 3);
    list_push_back(list1, 4);
    list_push_back(list1, 5);
    list_push_back(list1, 3);
    
    list_push_back(list2, 2);
    list_push_back(list2, 4);
    list_push_back(list2, 6);
    
    printf("List 1 before removal: ");
    list_print(list1);
    
    printf("List 2: ");
    list_print(list2);
    
    list_remove_all_present_in_other(list1, list2);
    
    printf("List 1 after removal: ");
    list_print(list1);
    
    list_destroy(list1);
    list_destroy(list2);
    
    return 0;
}