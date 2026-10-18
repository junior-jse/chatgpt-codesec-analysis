#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct WordNode {
    char *word;
    struct WordNode *next;
} WordNode;

WordNode *create_node(const char *word) {
    size_t len;
    WordNode *node;
    
    if (word == NULL) {
        return NULL;
    }
    
    len = strnlen(word, SIZE_MAX);
    if (len == SIZE_MAX) {
        return NULL;
    }
    
    node = (WordNode *)malloc(sizeof(WordNode));
    if (node == NULL) {
        return NULL;
    }
    
    node->word = (char *)malloc(len + 1);
    if (node->word == NULL) {
        free(node);
        return NULL;
    }
    
    if (len > 0) {
        memcpy(node->word, word, len);
    }
    node->word[len] = '\0';
    node->next = NULL;
    return node;
}

void free_list(WordNode *head) {
    WordNode *current = head;
    WordNode *next;
    
    while (current != NULL) {
        next = current->next;
        free(current->word);
        free(current);
        current = next;
    }
}

WordNode *remove_words(WordNode *head, const char *word_to_remove) {
    WordNode *current;
    WordNode *previous;
    WordNode *to_delete;
    
    if (head == NULL || word_to_remove == NULL) {
        return head;
    }
    
    current = head;
    previous = NULL;
    
    while (current != NULL) {
        if (strcmp(current->word, word_to_remove) == 0) {
            to_delete = current;
            
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
    WordNode *temp = NULL;
    size_t i;
    
    const char *words[] = {"apple", "banana", "cherry", "banana", "date", "elderberry"};
    size_t num_words = sizeof(words) / sizeof(words[0]);
    
    for (i = 0; i < num_words; i++) {
        temp = create_node(words[i]);
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