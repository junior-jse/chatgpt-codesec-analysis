#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

typedef struct {
    Node *head;
    Node *tail;
    size_t size;
} List;

void list_init(List *list) {
    if (list != NULL) {
        list->head = NULL;
        list->tail = NULL;
        list->size = 0;
    }
}

int list_append(List *list, int value) {
    Node *new_node;
    
    if (list == NULL) {
        return -1;
    }
    
    new_node = malloc(sizeof(*new_node));
    if (new_node == NULL) {
        return -1;
    }
    
    new_node->data = value;
    new_node->next = NULL;
    
    if (list->head == NULL) {
        list->head = new_node;
        list->tail = new_node;
    } else {
        list->tail->next = new_node;
        list->tail = new_node;
    }
    list->size++;
    
    return 0;
}

int list_split(List *source, size_t first_len, List *first, List *second) {
    Node *current;
    Node *split_node;
    size_t i;
    
    if (source == NULL || first == NULL || second == NULL) {
        return -1;
    }
    
    if (first == second) {
        return -1;
    }
    
    if (first_len > source->size) {
        return -1;
    }
    
    list_init(first);
    list_init(second);
    
    if (first_len == 0) {
        *second = *source;
        list_init(source);
        return 0;
    }
    
    if (first_len == source->size) {
        *first = *source;
        list_init(source);
        return 0;
    }
    
    current = source->head;
    for (i = 0; i < first_len - 1; i++) {
        current = current->next;
    }
    
    split_node = current->next;
    
    first->head = source->head;
    first->tail = current;
    first->size = first_len;
    current->next = NULL;
    
    second->head = split_node;
    second->tail = source->tail;
    second->size = source->size - first_len;
    
    list_init(source);
    
    return 0;
}

void list_destroy(List *list) {
    Node *current;
    Node *next;
    
    if (list == NULL) {
        return;
    }
    
    current = list->head;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
    
    list_init(list);
}

void list_print(const List *list) {
    Node *current;
    
    if (list == NULL) {
        printf("(null)\n");
        return;
    }
    
    current = list->head;
    while (current != NULL) {
        printf("%d", current->data);
        if (current->next != NULL) {
            printf(" -> ");
        }
        current = current->next;
    }
    printf(" (size: %zu)\n", list->size);
}

int main(void) {
    List original;
    List first_part;
    List second_part;
    int result;
    size_t i;
    
    list_init(&original);
    list_init(&first_part);
    list_init(&second_part);
    
    for (i = 1; i <= 10; i++) {
        if (list_append(&original, (int)(i * 10)) != 0) {
            fprintf(stderr, "Failed to append element\n");
            list_destroy(&original);
            return EXIT_FAILURE;
        }
    }
    
    printf("Original list: ");
    list_print(&original);
    
    result = list_split(&original, 4, &first_part, &second_part);
    if (result != 0) {
        fprintf(stderr, "Split operation failed\n");
        list_destroy(&original);
        return EXIT_FAILURE;
    }
    
    printf("First part:  ");
    list_print(&first_part);
    printf("Second part: ");
    list_print(&second_part);
    printf("Original after split: ");
    list_print(&original);
    
    list_destroy(&first_part);
    list_destroy(&second_part);
    
    return EXIT_SUCCESS;
}