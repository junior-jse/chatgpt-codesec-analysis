#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct Node {
    char *data;
    struct Node *next;
} Node;

typedef struct {
    Node *head;
    size_t size;
} StringList;

Node *create_node(const char *str) {
    Node *new_node = (Node *)malloc(sizeof(Node));
    if (!new_node) return NULL;
    
    new_node->data = (char *)malloc(strlen(str) + 1);
    if (!new_node->data) {
        free(new_node);
        return NULL;
    }
    
    strcpy(new_node->data, str);
    new_node->next = NULL;
    return new_node;
}

void list_init(StringList *list) {
    list->head = NULL;
    list->size = 0;
}

int list_add(StringList *list, const char *str) {
    Node *new_node = create_node(str);
    if (!new_node) return -1;
    
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
    return 0;
}

StringList *string_to_list(const char *str) {
    StringList *list = (StringList *)malloc(sizeof(StringList));
    if (!list) return NULL;
    
    list_init(list);
    
    if (str == NULL) {
        return list;
    }
    
    char *copy = (char *)malloc(strlen(str) + 1);
    if (!copy) {
        free(list);
        return NULL;
    }
    strcpy(copy, str);
    
    char *token = strtok(copy, " \t\n\r");
    while (token != NULL) {
        if (list_add(list, token) != 0) {
            free(copy);
            list_free(list);
            free(list);
            return NULL;
        }
        token = strtok(NULL, " \t\n\r");
    }
    
    free(copy);
    return list;
}

void list_free(StringList *list) {
    Node *current = list->head;
    while (current != NULL) {
        Node *next = current->next;
        free(current->data);
        free(current);
        current = next;
    }
    list->head = NULL;
    list->size = 0;
}

void list_print(const StringList *list) {
    Node *current = list->head;
    while (current != NULL) {
        printf("%s\n", current->data);
        current = current->next;
    }
}

int main(void) {
    const char *test_string = "hello world from C";
    StringList *list = string_to_list(test_string);
    
    if (list == NULL) {
        fprintf(stderr, "Failed to create list\n");
        return EXIT_FAILURE;
    }
    
    printf("String list (%zu items):\n", list->size);
    list_print(list);
    
    list_free(list);
    free(list);
    
    return EXIT_SUCCESS;
}