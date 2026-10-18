#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node* next;
} Node;

typedef struct List {
    Node* head;
    int length;
} List;

typedef struct ListContainer {
    List** lists;
    int count;
} ListContainer;

List* create_list(void) {
    List* list = (List*)malloc(sizeof(List));
    if (!list) return NULL;
    list->head = NULL;
    list->length = 0;
    return list;
}

void append_node(List* list, int data) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (!new_node) return;
    new_node->data = data;
    new_node->next = NULL;
    
    if (!list->head) {
        list->head = new_node;
    } else {
        Node* current = list->head;
        while (current->next) {
            current = current->next;
        }
        current->next = new_node;
    }
    list->length++;
}

void free_list(List* list) {
    if (!list) return;
    Node* current = list->head;
    while (current) {
        Node* temp = current;
        current = current->next;
        free(temp);
    }
    free(list);
}

List* find_longest_list(ListContainer* container) {
    if (!container || !container->lists || container->count <= 0) {
        return NULL;
    }
    
    List* longest = container->lists[0];
    int max_length = longest->length;
    
    for (int i = 1; i < container->count; i++) {
        if (container->lists[i]->length > max_length) {
            max_length = container->lists[i]->length;
            longest = container->lists[i];
        }
    }
    
    return longest;
}

List** find_lists_with_max_length(ListContainer* container, int* result_count) {
    if (!container || !container->lists || container->count <= 0 || !result_count) {
        *result_count = 0;
        return NULL;
    }
    
    int max_length = 0;
    for (int i = 0; i < container->count; i++) {
        if (container->lists[i]->length > max_length) {
            max_length = container->lists[i]->length;
        }
    }
    
    int count = 0;
    for (int i = 0; i < container->count; i++) {
        if (container->lists[i]->length == max_length) {
            count++;
        }
    }
    
    List** result = (List**)malloc(count * sizeof(List*));
    if (!result) {
        *result_count = 0;
        return NULL;
    }
    
    int index = 0;
    for (int i = 0; i < container->count; i++) {
        if (container->lists[i]->length == max_length) {
            result[index++] = container->lists[i];
        }
    }
    
    *result_count = count;
    return result;
}

int main(void) {
    ListContainer container;
    container.count = 4;
    container.lists = (List**)malloc(container.count * sizeof(List*));
    
    for (int i = 0; i < container.count; i++) {
        container.lists[i] = create_list();
    }
    
    append_node(container.lists[0], 1);
    append_node(container.lists[0], 2);
    append_node(container.lists[0], 3);
    
    append_node(container.lists[1], 1);
    append_node(container.lists[1], 2);
    
    append_node(container.lists[2], 1);
    append_node(container.lists[2], 2);
    append_node(container.lists[2], 3);
    
    append_node(container.lists[3], 1);
    append_node(container.lists[3], 2);
    append_node(container.lists[3], 3);
    append_node(container.lists[3], 4);
    
    int result_count = 0;
    List** longest_lists = find_lists_with_max_length(&container, &result_count);
    
    if (longest_lists) {
        printf("Longest list length: %d\n", longest_lists[0]->length);
        printf("Number of longest lists: %d\n", result_count);
        free(longest_lists);
    }
    
    for (int i = 0; i < container.count; i++) {
        free_list(container.lists[i]);
    }
    free(container.lists);
    
    return 0;
}