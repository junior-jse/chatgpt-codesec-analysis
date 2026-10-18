#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    char data;
    struct Node *next;
} Node;

int string_to_list(const char *str, Node **head_out) {
    if (str == NULL || head_out == NULL) {
        return -1;
    }

    *head_out = NULL;

    Node *head = NULL;
    Node *tail = NULL;

    for (size_t i = 0; str[i] != '\0'; i++) {
        Node *new_node = malloc(sizeof *new_node);
        if (new_node == NULL) {
            while (head != NULL) {
                Node *temp = head;
                head = head->next;
                free(temp);
            }
            return -1;
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

    *head_out = head;
    return 0;
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
        putchar(current->data);
        current = current->next;
    }
    putchar('\n');
}

int main(int argc, char *argv[]) {
    const char *str = (argc > 1) ? argv[1] : "Hello, World!";
    Node *list = NULL;

    if (string_to_list(str, &list) != 0) {
        fprintf(stderr, "Failed to convert string to list\n");
        return EXIT_FAILURE;
    }

    print_list(list);
    free_list(list);

    return EXIT_SUCCESS;
}