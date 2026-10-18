#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char *data;
    size_t data_len;
    struct Node *next;
} Node;

char *concatenate_list(Node *head) {
    size_t total_length = 0;
    Node *current = head;

    while (current != NULL) {
        total_length += current->data_len;
        current = current->next;
    }

    char *result = (char *)malloc(total_length + 1);
    if (result == NULL) {
        return NULL;
    }

    current = head;
    size_t offset = 0;
    while (current != NULL) {
        memcpy(result + offset, current->data, current->data_len);
        offset += current->data_len;
        current = current->next;
    }
    result[offset] = '\0';

    return result;
}

Node *create_node(const char *str) {
    size_t len = strlen(str);
    Node *node = (Node *)malloc(sizeof(Node));
    if (node == NULL) {
        return NULL;
    }

    node->data = (char *)malloc(len + 1);
    if (node->data == NULL) {
        free(node);
        return NULL;
    }

    memcpy(node->data, str, len);
    node->data[len] = '\0';
    node->data_len = len;
    node->next = NULL;

    return node;
}

void free_list(Node *head) {
    Node *current = head;
    while (current != NULL) {
        Node *next = current->next;
        free(current->data);
        free(current);
        current = next;
    }
}

int main(void) {
    Node *head = NULL;
    Node *tail = NULL;
    char *result;

    const char *words[] = {"Hello", " ", "World", "!", NULL};
    int i;

    for (i = 0; words[i] != NULL; i++) {
        Node *node = create_node(words[i]);
        if (node == NULL) {
            free_list(head);
            return 1;
        }
        if (head == NULL) {
            head = node;
            tail = node;
        } else {
            tail->next = node;
            tail = node;
        }
    }

    result = concatenate_list(head);
    if (result == NULL) {
        free_list(head);
        return 1;
    }

    printf("%s\n", result);

    free(result);
    free_list(head);

    return 0;
}