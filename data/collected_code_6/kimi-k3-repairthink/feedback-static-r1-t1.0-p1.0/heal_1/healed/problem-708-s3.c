#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRING_LENGTH 4096

typedef struct Node {
    char data;
    struct Node *next;
} Node;

void free_list(Node *head) {
    while (head != NULL) {
        Node *temp = head;
        head = head->next;
        free(temp);
    }
}

int string_to_list(const char *str, Node **out) {
    Node *head = NULL;
    Node *tail = NULL;
    size_t i;
    size_t len;

    if (str == NULL || out == NULL) {
        return -1;
    }

    *out = NULL;
    len = strnlen(str, MAX_STRING_LENGTH);

    for (i = 0; i < len; i++) {
        Node *new_node = malloc(sizeof *new_node);
        if (new_node == NULL) {
            free_list(head);
            return -1;
        }

        new_node->data = str[i];
        new_node->next = NULL;

        if (head == NULL) {
            head = new_node;
        } else {
            tail->next = new_node;
        }
        tail = new_node;
    }

    *out = head;
    return 0;
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
    Node *list = NULL;

    if (string_to_list(test_string, &list) != 0) {
        fprintf(stderr, "Failed to convert string to list\n");
        return EXIT_FAILURE;
    }

    print_list(list);
    free_list(list);

    return EXIT_SUCCESS;
}