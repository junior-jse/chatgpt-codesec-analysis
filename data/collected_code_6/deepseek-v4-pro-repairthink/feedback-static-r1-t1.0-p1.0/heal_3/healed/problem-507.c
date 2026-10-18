#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORD_LEN 1024

typedef struct node {
    char *word;
    struct node *next;
} node_t;

static size_t safe_strlen(const char *s, size_t maxlen) {
    size_t i;
    for (i = 0; i < maxlen && s[i] != '\0'; i++)
        ;
    return i;
}

node_t *create_node(const char *word) {
    if (word == NULL) {
        return NULL;
    }
    size_t len = safe_strlen(word, MAX_WORD_LEN);
    node_t *new_node = (node_t *)malloc(sizeof(node_t));
    if (new_node == NULL) {
        return NULL;
    }
    new_node->word = (char *)malloc(len + 1);
    if (new_node->word == NULL) {
        free(new_node);
        return NULL;
    }
    strncpy(new_node->word, word, len);
    new_node->word[len] = '\0';
    new_node->next = NULL;
    return new_node;
}

void free_list(node_t *head) {
    node_t *current = head;
    node_t *next;
    while (current != NULL) {
        next = current->next;
        free(current->word);
        free(current);
        current = next;
    }
}

node_t *remove_words(node_t *head, const char *target) {
    if (head == NULL || target == NULL) {
        return head;
    }
    node_t dummy;
    dummy.word = NULL;
    dummy.next = head;
    node_t *prev = &dummy;
    node_t *current = head;
    while (current != NULL) {
        if (strcmp(current->word, target) == 0) {
            prev->next = current->next;
            free(current->word);
            free(current);
            current = prev->next;
        } else {
            prev = current;
            current = current->next;
        }
    }
    return dummy.next;
}

void print_list(node_t *head) {
    node_t *current = head;
    while (current != NULL) {
        printf("%s ", current->word);
        current = current->next;
    }
    printf("\n");
}

int main(void) {
    node_t *head = NULL;
    node_t *tail = NULL;
    const char *words[] = {"apple", "banana", "apple", "cherry", "apple"};
    int count = sizeof(words) / sizeof(words[0]);
    int i;
    for (i = 0; i < count; i++) {
        node_t *new_node = create_node(words[i]);
        if (new_node == NULL) {
            free_list(head);
            return 1;
        }
        if (head == NULL) {
            head = new_node;
            tail = new_node;
        } else {
            tail->next = new_node;
            tail = new_node;
        }
    }
    printf("Original list: ");
    print_list(head);
    head = remove_words(head, "apple");
    printf("After removing 'apple': ");
    print_list(head);
    free_list(head);
    return 0;
}