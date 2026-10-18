#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char **words;
    size_t count;
    size_t capacity;
} WordList;

int init_word_list(WordList *list, size_t initial_capacity) {
    if (list == NULL || initial_capacity == 0) {
        return -1;
    }
    
    list->words = malloc(initial_capacity * sizeof(char *));
    if (list->words == NULL) {
        return -1;
    }
    
    list->count = 0;
    list->capacity = initial_capacity;
    return 0;
}

int add_word(WordList *list, const char *word) {
    if (list == NULL || word == NULL) {
        return -1;
    }
    
    if (list->count >= list->capacity) {
        size_t new_capacity = list->capacity * 2;
        char **new_words = realloc(list->words, new_capacity * sizeof(char *));
        if (new_words == NULL) {
            return -1;
        }
        list->words = new_words;
        list->capacity = new_capacity;
    }
    
    list->words[list->count] = malloc(strlen(word) + 1);
    if (list->words[list->count] == NULL) {
        return -1;
    }
    
    strcpy(list->words[list->count], word);
    list->count++;
    return 0;
}

void remove_word(WordList *list, const char *word) {
    if (list == NULL || word == NULL || list->count == 0) {
        return;
    }
    
    size_t write_index = 0;
    
    for (size_t i = 0; i < list->count; i++) {
        if (strcmp(list->words[i], word) != 0) {
            if (write_index != i) {
                list->words[write_index] = list->words[i];
            }
            write_index++;
        } else {
            free(list->words[i]);
        }
    }
    
    list->count = write_index;
}

void free_word_list(WordList *list) {
    if (list == NULL) {
        return;
    }
    
    if (list->words != NULL) {
        for (size_t i = 0; i < list->count; i++) {
            free(list->words[i]);
        }
        free(list->words);
        list->words = NULL;
    }
    
    list->count = 0;
    list->capacity = 0;
}

void print_word_list(const WordList *list) {
    if (list == NULL) {
        return;
    }
    
    for (size_t i = 0; i < list->count; i++) {
        printf("%s", list->words[i]);
        if (i < list->count - 1) {
            printf(" ");
        }
    }
    printf("\n");
}

int main(void) {
    WordList list;
    
    if (init_word_list(&list, 10) != 0) {
        fprintf(stderr, "Failed to initialize word list\n");
        return EXIT_FAILURE;
    }
    
    if (add_word(&list, "apple") != 0 ||
        add_word(&list, "banana") != 0 ||
        add_word(&list, "cherry") != 0 ||
        add_word(&list, "banana") != 0 ||
        add_word(&list, "date") != 0) {
        fprintf(stderr, "Failed to add words\n");
        free_word_list(&list);
        return EXIT_FAILURE;
    }
    
    printf("Original list: ");
    print_word_list(&list);
    
    remove_word(&list, "banana");
    
    printf("After removing 'banana': ");
    print_word_list(&list);
    
    remove_word(&list, "apple");
    
    printf("After removing 'apple': ");
    print_word_list(&list);
    
    free_word_list(&list);
    
    return EXIT_SUCCESS;
}