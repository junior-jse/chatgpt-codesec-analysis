#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char *data;
    struct Node *next;
} Node;

Node *create_node(const char *str) {
    Node *node = (Node *)malloc(sizeof(Node));
    if (!node) return NULL;
    node->data = (char *)malloc(strlen(str) + 1);
    if (!node->data) {
        free(node);
        return NULL;
    }
    strcpy(node->data, str);
    node->next = NULL;
    return node;
}

Node *string_to_list(const char *str) {
    if (!str) return NULL;
    
    Node *head = NULL;
    Node *tail = NULL;
    const char *start = str;
    const char *p = str;
    char *token;
    
    while (*p) {
        if (*p == ' ' || *p == '\t' || *p == '\n' || *p == ',') {
            if (p > start) {
                size_t len = p - start;
                token = (char *)malloc(len + 1);
                if (!token) {
                    Node *current = head;
                    while (current) {
                        Node *next = current->next;
                        free(current->data);
                        free(current);
                        current = next;
                    }
                    return NULL;
                }
                strncpy(token, start, len);
                token[len] = '\0';
                
                Node *node = create_node(token);
                free(token);
                if (!node) {
                    Node *current = head;
                    while (current) {
                        Node *next = current->next;
                        free(current->data);
                        free(current);
                        current = next;
                    }
                    return NULL;
                }
                
                if (!head) {
                    head = node;
                    tail = node;
                } else {
                    tail->next = node;
                    tail = node;
                }
            }
            start = p + 1;
        }
        p++;
    }
    
    if (p > start) {
        size_t len = p - start;
        token = (char *)malloc(len + 1);
        if (!token) {
            Node *current = head;
            while (current) {
                Node *next = current->next;
                free(current->data);
                free(current);
                current = next;
            }
            return NULL;
        }
        strncpy(token, start, len);
        token[len] = '\0';
        
        Node *node = create_node(token);
        free(token);
        if (!node) {
            Node *current = head;
            while (current) {
                Node *next = current->next;
                free(current->data);
                free(current);
                current = next;
            }
            return NULL;
        }
        
        if (!head) {
            head = node;
            tail = node;
        } else {
            tail->next = node;
            tail = node;
        }
    }
    
    return head;
}

void free_list(Node *head) {
    Node *current = head;
    while (current) {
        Node *next = current->next;
        free(current->data);
        free(current);
        current = next;
    }
}

void print_list(Node *head) {
    Node *current = head;
    while (current) {
        printf("%s\n", current->data);
        current = current->next;
    }
}

int main(void) {
    const char *input = "hello world foo bar";
    Node *list = string_to_list(input);
    
    if (list) {
        print_list(list);
        free_list(list);
    } else {
        fprintf(stderr, "Error converting string to list\n");
        return 1;
    }
    
    return 0;
}