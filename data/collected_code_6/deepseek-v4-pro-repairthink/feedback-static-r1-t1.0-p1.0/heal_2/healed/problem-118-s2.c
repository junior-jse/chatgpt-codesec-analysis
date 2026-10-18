#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char *data;
    struct Node *next;
} Node;

Node *create_node(const char *str, size_t len) {
    if (!str) return NULL;

    Node *new_node = (Node *)malloc(sizeof(Node));
    if (!new_node) return NULL;

    new_node->data = (char *)malloc(len + 1);
    if (!new_node->data) {
        free(new_node);
        return NULL;
    }

    memcpy(new_node->data, str, len);
    new_node->data[len] = '\0';
    new_node->next = NULL;
    return new_node;
}

Node *string_to_list(const char *str, size_t str_len, const char *delimiter) {
    if (!str || !delimiter) return NULL;

    char *copy = (char *)malloc(str_len + 1);
    if (!copy) return NULL;

    memcpy(copy, str, str_len);
    copy[str_len] = '\0';

    Node *head = NULL;
    Node *tail = NULL;

    char *token = strtok(copy, delimiter);
    while (token) {
        size_t token_len = strlen(token);
        Node *new_node = create_node(token, token_len);
        if (!new_node) {
            Node *current = head;
            while (current) {
                Node *next = current->next;
                free(current->data);
                free(current);
                current = next;
            }
            free(copy);
            return NULL;
        }

        if (!head) {
            head = new_node;
            tail = new_node;
        } else {
            tail->next = new_node;
            tail = new_node;
        }

        token = strtok(NULL, delimiter);
    }

    free(copy);
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
    const char *str = "hello,world,example,test";
    const char *delimiter = ",";

    Node *list = string_to_list(str, strlen(str), delimiter);
    if (!list) {
        fprintf(stderr, "Error: Failed to create list\n");
        return 1;
    }

    print_list(list);
    free_list(list);

    return 0;
}