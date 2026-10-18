#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct Node {
    char *data;
    struct Node *next;
} Node;

static Node *create_node(const char *str, size_t len) {
    Node *node;
    char *data;
    
    if (!str || len == 0) return NULL;
    
    if (len >= SIZE_MAX) return NULL;
    
    node = (Node *)malloc(sizeof(Node));
    if (!node) return NULL;
    
    data = (char *)malloc(len + 1);
    if (!data) {
        free(node);
        return NULL;
    }
    
    memcpy(data, str, len);
    data[len] = '\0';
    node->data = data;
    node->next = NULL;
    return node;
}

static void cleanup_list(Node *head) {
    Node *current = head;
    while (current) {
        Node *next = current->next;
        free(current->data);
        free(current);
        current = next;
    }
}

static size_t span_token(const char *str) {
    size_t len = 0;
    while (str[len] && str[len] != ' ' && str[len] != '\t' && 
           str[len] != '\n' && str[len] != ',') {
        len++;
    }
    return len;
}

static size_t span_delimiter(const char *str) {
    size_t len = 0;
    while (str[len] && (str[len] == ' ' || str[len] == '\t' || 
           str[len] == '\n' || str[len] == ',')) {
        len++;
    }
    return len;
}

Node *string_to_list(const char *str) {
    if (!str) return NULL;
    
    Node *head = NULL;
    Node *tail = NULL;
    const char *p = str;
    
    while (*p) {
        size_t delim_len = span_delimiter(p);
        p += delim_len;
        
        if (*p) {
            size_t token_len = span_token(p);
            if (token_len > 0) {
                Node *node = create_node(p, token_len);
                if (!node) {
                    cleanup_list(head);
                    return NULL;
                }
                
                if (!head) {
                    head = node;
                    tail = node;
                } else {
                    tail->next = node;
                    tail = node;
                }
                p += token_len;
            }
        }
    }
    
    return head;
}

void free_list(Node *head) {
    cleanup_list(head);
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