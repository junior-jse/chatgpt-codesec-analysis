#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

typedef struct List {
    Node *head;
    int size;
} List;

typedef struct SplitResult {
    List **lists;
    int count;
} SplitResult;

Node *create_node(int data) {
    Node *node = (Node *)malloc(sizeof(Node));
    if (!node) return NULL;
    node->data = data;
    node->next = NULL;
    return node;
}

List *create_list(void) {
    List *list = (List *)malloc(sizeof(List));
    if (!list) return NULL;
    list->head = NULL;
    list->size = 0;
    return list;
}

int list_append(List *list, int data) {
    Node *node = create_node(data);
    if (!node) return 0;
    
    if (!list->head) {
        list->head = node;
    } else {
        Node *current = list->head;
        while (current->next) {
            current = current->next;
        }
        current->next = node;
    }
    list->size++;
    return 1;
}

void list_free(List *list) {
    if (!list) return;
    Node *current = list->head;
    while (current) {
        Node *next = current->next;
        free(current);
        current = next;
    }
    free(list);
}

SplitResult split_list(List *source, int n) {
    SplitResult result;
    result.lists = NULL;
    result.count = 0;
    
    if (!source || n <= 0 || source->size == 0) {
        return result;
    }
    
    int num_lists = (source->size + n - 1) / n;
    result.lists = (List **)malloc(sizeof(List *) * num_lists);
    if (!result.lists) {
        return result;
    }
    
    for (int i = 0; i < num_lists; i++) {
        result.lists[i] = create_list();
        if (!result.lists[i]) {
            for (int j = 0; j < i; j++) {
                list_free(result.lists[j]);
            }
            free(result.lists);
            result.lists = NULL;
            result.count = 0;
            return result;
        }
    }
    
    result.count = num_lists;
    
    Node *current = source->head;
    int index = 0;
    
    while (current) {
        int list_index = index / n;
        Node *next = current->next;
        current->next = NULL;
        
        if (!result.lists[list_index]->head) {
            result.lists[list_index]->head = current;
        } else {
            Node *tail = result.lists[list_index]->head;
            while (tail->next) {
                tail = tail->next;
            }
            tail->next = current;
        }
        result.lists[list_index]->size++;
        
        current = next;
        index++;
    }
    
    source->head = NULL;
    source->size = 0;
    
    return result;
}

void split_result_free(SplitResult *result) {
    if (!result) return;
    for (int i = 0; i < result->count; i++) {
        list_free(result->lists[i]);
    }
    free(result->lists);
    result->lists = NULL;
    result->count = 0;
}

void print_list(List *list) {
    if (!list) {
        printf("NULL\n");
        return;
    }
    Node *current = list->head;
    printf("[");
    while (current) {
        printf("%d", current->data);
        if (current->next) printf(", ");
        current = current->next;
    }
    printf("]\n");
}

int main(void) {
    List *list = create_list();
    if (!list) {
        fprintf(stderr, "Failed to create list\n");
        return 1;
    }
    
    for (int i = 1; i <= 10; i++) {
        if (!list_append(list, i)) {
            fprintf(stderr, "Failed to append element\n");
            list_free(list);
            return 1;
        }
    }
    
    printf("Original list: ");
    print_list(list);
    
    SplitResult result = split_list(list, 3);
    if (result.count == 0 && result.lists == NULL) {
        fprintf(stderr, "Failed to split list\n");
        list_free(list);
        return 1;
    }
    
    printf("Split into %d lists (every 3 elements):\n", result.count);
    for (int i = 0; i < result.count; i++) {
        printf("List %d: ", i + 1);
        print_list(result.lists[i]);
    }
    
    split_result_free(&result);
    list_free(list);
    
    return 0;
}