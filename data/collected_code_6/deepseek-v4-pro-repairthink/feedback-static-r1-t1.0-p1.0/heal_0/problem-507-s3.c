#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct WordNode {
    char *word;
    struct WordNode *next;
} WordNode;

typedef struct {
    WordNode *head;
    size_t size;
} WordList;

void wordlist_init(WordList *list) {
    if (list == NULL) return;
    list->head = NULL;
    list->size = 0;
}

int wordlist_add(WordList *list, const char *word) {
    WordNode *new_node;
    WordNode *current;
    char *word_copy;
    
    if (list == NULL || word == NULL) return 0;
    
    word_copy = strdup(word);
    if (word_copy == NULL) return 0;
    
    new_node = (WordNode *)malloc(sizeof(WordNode));
    if (new_node == NULL) {
        free(word_copy);
        return 0;
    }
    
    new_node->word = word_copy;
    new_node->next = NULL;
    
    if (list->head == NULL) {
        list->head = new_node;
    } else {
        current = list->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
    }
    
    list->size++;
    return 1;
}

void wordlist_remove_words(WordList *list, const char **words_to_remove, size_t remove_count) {
    WordNode *current;
    WordNode *prev;
    WordNode *to_delete;
    size_t i;
    
    if (list == NULL || words_to_remove == NULL || remove_count == 0) return;
    
    current = list->head;
    prev = NULL;
    
    while (current != NULL) {
        int match_found = 0;
        
        for (i = 0; i < remove_count; i++) {
            if (words_to_remove[i] != NULL && strcmp(current->word, words_to_remove[i]) == 0) {
                match_found = 1;
                break;
            }
        }
        
        if (match_found) {
            to_delete = current;
            if (prev == NULL) {
                list->head = current->next;
                current = list->head;
            } else {
                prev->next = current->next;
                current = current->next;
            }
            free(to_delete->word);
            free(to_delete);
            list->size--;
        } else {
            prev = current;
            current = current->next;
        }
    }
}

void wordlist_print(const WordList *list) {
    const WordNode *current;
    
    if (list == NULL) return;
    
    current = list->head;
    while (current != NULL) {
        printf("%s ", current->word);
        current = current->next;
    }
    printf("\n");
}

void wordlist_destroy(WordList *list) {
    WordNode *current;
    WordNode *next;
    
    if (list == NULL) return;
    
    current = list->head;
    while (current != NULL) {
        next = current->next;
        free(current->word);
        free(current);
        current = next;
    }
    
    list->head = NULL;
    list->size = 0;
}

int main(void) {
    WordList list;
    const char *words_to_remove[] = {"the", "and", "or", "but"};
    size_t remove_count = sizeof(words_to_remove) / sizeof(words_to_remove[0]);
    
    wordlist_init(&list);
    
    wordlist_add(&list, "the");
    wordlist_add(&list, "quick");
    wordlist_add(&list, "brown");
    wordlist_add(&list, "and");
    wordlist_add(&list, "fox");
    wordlist_add(&list, "or");
    wordlist_add(&list, "jumps");
    wordlist_add(&list, "but");
    wordlist_add(&list, "over");
    
    printf("Original list: ");
    wordlist_print(&list);
    
    wordlist_remove_words(&list, words_to_remove, remove_count);
    
    printf("After removal: ");
    wordlist_print(&list);
    
    wordlist_destroy(&list);
    
    return 0;
}