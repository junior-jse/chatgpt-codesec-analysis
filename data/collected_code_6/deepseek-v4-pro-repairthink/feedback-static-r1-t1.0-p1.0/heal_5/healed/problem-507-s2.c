#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORD_LEN 1024

typedef struct WordNode {
    char *word;
    struct WordNode *next;
} WordNode;

WordNode *create_node(const char *word) {
    if (word == NULL) return NULL;

    size_t len = 0;
    while (len < MAX_WORD_LEN && word[len] != '\0') {
        len++;
    }
    if (len == MAX_WORD_LEN && word[len] != '\0') {
        return NULL;
    }

    WordNode *node = (WordNode *)malloc(sizeof(WordNode));
    if (node == NULL) {
        return NULL;
    }

    node->word = (char *)malloc(len + 1);
    if (node->word == NULL) {
        free(node);
        return NULL;
    }

    strncpy(node->word, word, len);
    node->word[len] = '\0';
    node->next = NULL;
    return node;
}

void free_list(WordNode *head) {
    WordNode *current = head;
    while (current != NULL) {
        WordNode *next = current->next;
        free(current->word);
        free(current);
        current = next;
    }
}

WordNode *remove_words(WordNode *head, const char *word_to_remove) {
    if (head == NULL || word_to_remove == NULL) {
        return head;
    }

    WordNode *current = head;
    WordNode *previous = NULL;

    while (current != NULL) {
        if (strcmp(current->word, word_to_remove) == 0) {
            WordNode *to_delete = current;
            if (previous == NULL) {
                head = current->next;
            } else {
                previous->next = current->next;
            }
            current = current->next;
            free(to_delete->word);
            free(to_delete);
        } else {
            previous = current;
            current = current->next;
        }
    }

    return head;
}

void print_list(WordNode *head) {
    WordNode *current = head;
    while (current != NULL) {
        printf("%s ", current->word);
        current = current->next;
    }
    printf("\n");
}

int main(void) {
    WordNode *list = NULL;

    const char *words[] = {"apple", "banana", "cherry", "banana", "date", "elderberry"};
    int num_words = sizeof(words) / sizeof(words[0]);

    for (int i = 0; i < num_words; i++) {
        WordNode *temp = create_node(words[i]);
        if (temp == NULL) {
            free_list(list);
            return 1;
        }
        temp->next = list;
        list = temp;
    }

    printf("Original list: ");
    print_list(list);

    list = remove_words(list, "banana");

    printf("After removing 'banana': ");
    print_list(list);

    free_list(list);

    return 0;
}