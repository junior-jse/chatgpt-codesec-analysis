#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

typedef struct {
    Node *head;
    int length;
} SublistResult;

Node* create_node(int data) {
    Node *new_node = malloc(sizeof(Node));
    if (new_node == NULL) {
        return NULL;
    }
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}

void free_list(Node *head) {
    Node *current = head;
    while (current != NULL) {
        Node *temp = current;
        current = current->next;
        free(temp);
    }
}

SublistResult find_max_length_sublist(Node *head, int target) {
    SublistResult result = {NULL, 0};
    
    if (head == NULL) {
        return result;
    }
    
    Node *current = head;
    Node *max_start = NULL;
    int max_len = 0;
    int current_len = 0;
    Node *current_start = NULL;
    
    while (current != NULL) {
        if (current->data == target) {
            if (current_len == 0) {
                current_start = current;
            }
            current_len++;
        } else {
            if (current_len > max_len) {
                max_len = current_len;
                max_start = current_start;
            }
            current_len = 0;
            current_start = NULL;
        }
        current = current->next;
    }
    
    if (current_len > max_len) {
        max_len = current_len;
        max_start = current_start;
    }
    
    result.head = max_start;
    result.length = max_len;
    return result;
}

void print_list(Node *head) {
    Node *current = head;
    while (current != NULL) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");
}

int main(void) {
    Node *head = NULL;
    Node *tail = NULL;
    int values[] = {1, 2, 2, 3, 2, 2, 2, 4, 2, 2};
    int n = sizeof(values) / sizeof(values[0]);
    int target = 2;
    
    for (int i = 0; i < n; i++) {
        Node *new_node = create_node(values[i]);
        if (new_node == NULL) {
            free_list(head);
            return EXIT_FAILURE;
        }
        
        if (head == NULL) {
            head = new_node;
            tail = new_node;
        } else {
            tail->next = new_node;
            tail = new_node;
        }
    }
    
    printf("Original list: ");
    print_list(head);
    
    SublistResult result = find_max_length_sublist(head, target);
    
    printf("Maximum sublist of %d (length %d): ", target, result.length);
    if (result.head != NULL) {
        Node *current = result.head;
        int count = 0;
        while (current != NULL && count < result.length) {
            printf("%d ", current->data);
            current = current->next;
            count++;
        }
    }
    printf("\n");
    
    free_list(head);
    return EXIT_SUCCESS;
}