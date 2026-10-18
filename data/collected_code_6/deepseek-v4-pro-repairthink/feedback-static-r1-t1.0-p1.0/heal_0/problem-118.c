#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char *data;
    struct Node *next;
} Node;

Node *string_to_list(const char *str) {
    if (str == NULL) {
        return NULL;
    }

    size_t len = strlen(str);
    if (len == 0) {
        return NULL;
    }

    Node *head = NULL;
    Node *tail = NULL;
    size_t start = 0;
    size_t end = 0;

    while (end <= len) {
        if (str[end] == ' ' || str[end] == '\0') {
            if (end > start) {
                size_t word_len = end - start;
                char *word = (char *)malloc(word_len + 1);
                if (word == NULL) {
                    Node *current = head;
                    while (current != NULL) {
                        Node *next = current->next;
                        free(current->data);
                        free(current);
                        current = next;
                    }
                    return NULL;
                }
                strncpy(word, str + start, word_len);
                word[word_len] = '\0';

                Node *new_node = (Node *)malloc(sizeof(Node));
                if (new_node == NULL) {
                    free(word);
                    Node *current = head;
                    while (current != NULL) {
                        Node *next = current->next;
                        free(current->data);
                        free(current);
                        current = next;
                    }
                    return NULL;
                }

                new_node->data = word;
                new_node->next = NULL;

                if (head == NULL) {
                    head = new_node;
                    tail = new_node;
                } else {
                    tail->next = new_node;
                    tail = new_node;
                }
            }
            start = end + 1;
        }
        end++;
    }

    return head;
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

void print_list(Node *head) {
    Node *current = head;
    while (current != NULL) {
        printf("%s\n", current->data);
        current = current->next;
    }
}

int main(void) {
    const char *test_str = "Hello world from C";
    Node *list = string_to_list(test_str);

    if (list != NULL) {
        print_list(list);
        free_list(list);
    }

    return 0;
}