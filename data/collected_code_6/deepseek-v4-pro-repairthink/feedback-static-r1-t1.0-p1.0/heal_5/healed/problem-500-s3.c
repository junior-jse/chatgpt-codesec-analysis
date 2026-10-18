#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct Node {
    char *data;
    size_t data_len;
    struct Node *next;
} Node;

char *concatenate_list(Node *head) {
    size_t total_length = 0;
    Node *current = head;

    while (current != NULL) {
        if (current->data_len > SIZE_MAX - total_length) {
            return NULL;
        }
        total_length += current->data_len;
        current = current->next;
    }

    if (total_length > SIZE_MAX - 1) {
        return NULL;
    }

    char *result = (char *)malloc(total_length + 1);
    if (result == NULL) {
        return NULL;
    }

    current = head;
    size_t offset = 0;
    while (current != NULL) {
        size_t remaining = total_length - offset;
        if (current->data_len > remaining) {
            free(result);
            return NULL;
        }
        for (size_t i = 0; i < current->data_len; i++) {
            result[offset + i] = current->data[i];
        }
        offset += current->data_len;
        current = current->next;
    }
    result[offset] = '\0';
    return result;
}

Node *create_node(const char *str, size_t len) {
    if (str == NULL) {
        return NULL;
    }
    Node *node = (Node *)malloc(sizeof(Node));
    if (node == NULL) {
        return NULL;
    }

    node->data = (char *)malloc(len + 1);
    if (node->data == NULL) {
        free(node);
        return NULL;
    }

    for (size_t i = 0; i < len; i++) {
        node->data[i] = str[i];
    }
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

    const char *words_str[] = {"Hello", " ", "World", "!"};
    const size_t words_len[] = {
        sizeof("Hello") - 1,
        sizeof(" ") - 1,
        sizeof("World") - 1,
        sizeof("!") - 1
    };
    size_t num_words = sizeof(words_str) / sizeof(words_str[0]);

    for (size_t i = 0; i < num_words; i++) {
        Node *node = create_node(words_str[i], words_len[i]);
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