#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct ListNode {
    char *str;
    struct ListNode *next;
} ListNode;

ListNode *create_node(const char *str) {
    ListNode *node = (ListNode *)malloc(sizeof(ListNode));
    if (node == NULL) {
        return NULL;
    }

    size_t len = strnlen(str, 1024);
    node->str = (char *)malloc(len + 1);
    if (node->str == NULL) {
        free(node);
        return NULL;
    }
    memcpy(node->str, str, len);
    node->str[len] = '\0';
    node->next = NULL;
    return node;
}

void free_list(ListNode *head) {
    ListNode *current = head;
    while (current != NULL) {
        ListNode *next = current->next;
        free(current->str);
        free(current);
        current = next;
    }
}

ListNode *string_to_list(const char *input, const char *delimiter) {
    if (input == NULL || delimiter == NULL) {
        return NULL;
    }

    size_t input_len = strnlen(input, 65536);
    char *copy = (char *)malloc(input_len + 1);
    if (copy == NULL) {
        return NULL;
    }
    memcpy(copy, input, input_len);
    copy[input_len] = '\0';

    ListNode *head = NULL;
    ListNode *tail = NULL;

    char *saveptr;
    char *token = strtok_r(copy, delimiter, &saveptr);
    while (token != NULL) {
        ListNode *node = create_node(token);
        if (node == NULL) {
            free_list(head);
            free(copy);
            return NULL;
        }

        if (head == NULL) {
            head = node;
            tail = node;
        } else {
            tail->next = node;
            tail = node;
        }

        token = strtok_r(NULL, delimiter, &saveptr);
    }

    free(copy);
    return head;
}

void print_list(const ListNode *head) {
    const ListNode *current = head;
    while (current != NULL) {
        printf("%s\n", current->str);
        current = current->next;
    }
}

int main(void) {
    const char *input = "apple,banana,cherry,date";
    const char *delimiter = ",";

    ListNode *list = string_to_list(input, delimiter);
    if (list == NULL) {
        fprintf(stderr, "Failed to convert string to list\n");
        return EXIT_FAILURE;
    }

    print_list(list);
    free_list(list);

    return EXIT_SUCCESS;
}