#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char *data;
    struct Node *next;
} Node;

Node *create_node(const char *str, size_t len) {
    Node *node = (Node *)malloc(sizeof(Node));
    if (!node) return NULL;
    size_t alloc_size = len + 1;
    if (alloc_size < len) {
        free(node);
        return NULL;
    }
    node->data = (char *)malloc(alloc_size);
    if (!node->data) {
        free(node);
        return NULL;
    }
    for (size_t i = 0; i < len; i++) {
        node->data[i] = str[i];
    }
    node->data[len] = '\0';
    node->next = NULL;
    return node;
}

Node *string_to_list(const char *str) {
    if (!str) return NULL;
    Node *head = NULL;
    Node *tail = NULL;
    const char *p = str;
    while (*p) {
        while (*p == ' ' || *p == '\t' || *p == '\n' || *p == ',')
            p++;
        if (*p == '\0') break;
        const char *start = p;
        while (*p && *p != ' ' && *p != '\t' && *p != '\n' && *p != ',')
            p++;
        size_t len = p - start;
        Node *node = create_node(start, len);
        if (!node) {
            while (head) {
                Node *next = head->next;
                free(head->data);
                free(head);
                head = next;
            }
            return NULL;
        }
        if (!head) {
            head = tail = node;
        } else {
            tail->next = node;
            tail = node;
        }
    }
    return head;
}

void free_list(Node *head) {
    while (head) {
        Node *next = head->next;
        free(head->data);
        free(head);
        head = next;
    }
}

void print_list(Node *head) {
    while (head) {
        printf("%s\n", head->data);
        head = head->next;
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