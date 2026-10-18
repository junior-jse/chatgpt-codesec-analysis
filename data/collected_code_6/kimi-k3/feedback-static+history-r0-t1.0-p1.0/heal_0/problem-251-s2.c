#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

Node* create_node(int data) {
    Node *new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL) {
        return NULL;
    }
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}

int insert_before_each(Node **head, int element) {
    if (head == NULL) {
        return -1;
    }
    
    if (*head == NULL) {
        return 0;
    }
    
    Node *current = *head;
    Node *new_node;
    
    while (current != NULL) {
        new_node = create_node(element);
        if (new_node == NULL) {
            return -1;
        }
        
        new_node->next = current;
        
        if (current == *head) {
            *head = new_node;
        } else {
            Node *prev = *head;
            while (prev->next != current) {
                prev = prev->next;
            }
            prev->next = new_node;
        }
        
        current = current->next;
    }
    
    return 0;
}

void print_list(Node *head) {
    Node *current = head;
    while (current != NULL) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");
}

void free_list(Node **head) {
    if (head == NULL) {
        return;
    }
    
    Node *current = *head;
    Node *next;
    
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
    
    *head = NULL;
}

int main(void) {
    Node *head = NULL;
    int result;
    
    result = insert_before_each(&head, 0);
    if (result != 0) {
        fprintf(stderr, "Insert operation failed\n");
        return EXIT_FAILURE;
    }
    
    for (int i = 1; i <= 5; i++) {
        Node *new_node = create_node(i * 10);
        if (new_node == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            free_list(&head);
            return EXIT_FAILURE;
        }
        
        if (head == NULL) {
            head = new_node;
        } else {
            Node *current = head;
            while (current->next != NULL) {
                current = current->next;
            }
            current->next = new_node;
        }
    }
    
    printf("Original list: ");
    print_list(head);
    
    result = insert_before_each(&head, 99);
    if (result != 0) {
        fprintf(stderr, "Insert operation failed\n");
        free_list(&head);
        return EXIT_FAILURE;
    }
    
    printf("List after insertion: ");
    print_list(head);
    
    free_list(&head);
    
    return EXIT_SUCCESS;
}