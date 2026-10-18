#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char data;
    struct Node *next;
} Node;

Node* string_to_list(const char *str) {
    Node *head = NULL;
    Node *tail = NULL;
    size_t i;
    size_t len;

    if (str == NULL) {
        return NULL;
    }

    len = strlen(str);
    
    for (i = 0; i < len; i++) {
        Node *new_node = (Node*)malloc(sizeof(Node));
        if (new_node == NULL) {
            while (head != NULL) {
                Node *temp = head;
                head = head->next;
                free(temp);
            }
            return NULL;
        }
        
        new_node->data = str[i];
        new_node->next = NULL;
        
        if (head == NULL) {
            head = new_node;
            tail = new_node;
        } else {
            tail->next = new_node;
            tail = new_node;
        }
    }
    
    return head;
}

void free_list(Node *head) {
    while (head != NULL) {
        Node *temp = head;
        head = head->next;
        free(temp);
    }
}

void print_list(const Node *head) {
    const Node *current = head;
    while (current != NULL) {
        printf("%c", current->data);
        current = current->next;
    }
    printf("\n");
}

int main(void) {
    const char *test_string = "Hello, World!";
    Node *list = string_to_list(test_string);
    
    if (list == NULL && strlen(test_string) > 0) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }
    
    print_list(list);
    free_list(list);
    
    return EXIT_SUCCESS;
}