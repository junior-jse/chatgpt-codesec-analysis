#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char data;
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

int list_append(List *list, char data) {
    if (list == NULL) {
        return 0;
    }
    Node *new_node = (Node *)malloc(sizeof(Node));
    if (new_node == NULL) {
        return 0;
    }
    new_node->data = data;
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
    return 1;
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

List *string_to_list(const char *str) {
    if (str == NULL) {
        return NULL;
    }
    List *list = list_create();
    if (list == NULL) {
        return NULL;
    }
    size_t len = strlen(str);
    for (size_t i = 0; i < len; i++) {
        if (!list_append(list, str[i])) {
            list_destroy(list);
            return NULL;
        }
    }
    return list;
}

void list_print(const List *list) {
    if (list == NULL) {
        return;
    }
    Node *current = list->head;
    while (current != NULL) {
        printf("%c", current->data);
        current = current->next;
    }
    printf("\n");
}

int main(void) {
    const char *text = "Hello, World!";
    List *list = string_to_list(text);
    if (list == NULL) {
        fprintf(stderr, "Failed to convert string to list\n");
        return EXIT_FAILURE;
    }
    
    list_print(list);
    printf("List size: %zu\n", list->size);
    
    list_destroy(list);
    return EXIT_SUCCESS;
}