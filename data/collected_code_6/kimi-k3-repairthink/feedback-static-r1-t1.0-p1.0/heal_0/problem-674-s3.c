#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORDS 1000
#define MAX_WORD_LEN 100

typedef struct {
    char **words;
    size_t count;
    size_t capacity;
} WordCollection;

WordCollection* create_collection(size_t initial_capacity) {
    WordCollection *collection = malloc(sizeof(WordCollection));
    if (collection == NULL) {
        return NULL;
    }
    
    collection->words = malloc(initial_capacity * sizeof(char*));
    if (collection->words == NULL) {
        free(collection);
        return NULL;
    }
    
    collection->count = 0;
    collection->capacity = initial_capacity;
    return collection;
}

void free_collection(WordCollection *collection) {
    if (collection == NULL) {
        return;
    }
    
    for (size_t i = 0; i < collection->count; i++) {
        free(collection->words[i]);
    }
    free(collection->words);
    free(collection);
}

int word_exists(WordCollection *collection, const char *word) {
    if (collection == NULL || word == NULL) {
        return 0;
    }
    
    for (size_t i = 0; i < collection->count; i++) {
        if (strcmp(collection->words[i], word) == 0) {
            return 1;
        }
    }
    return 0;
}

int add_word(WordCollection *collection, const char *word) {
    if (collection == NULL || word == NULL) {
        return -1;
    }
    
    if (word_exists(collection, word)) {
        return 0;
    }
    
    if (collection->count >= collection->capacity) {
        size_t new_capacity = collection->capacity * 2;
        char **new_words = realloc(collection->words, new_capacity * sizeof(char*));
        if (new_words == NULL) {
            return -1;
        }
        collection->words = new_words;
        collection->capacity = new_capacity;
    }
    
    size_t word_len = strlen(word);
    collection->words[collection->count] = malloc((word_len + 1) * sizeof(char));
    if (collection->words[collection->count] == NULL) {
        return -1;
    }
    
    strcpy(collection->words[collection->count], word);
    collection->count++;
    return 1;
}

char* remove_duplicate_words(const char *input) {
    if (input == NULL) {
        return NULL;
    }
    
    WordCollection *collection = create_collection(10);
    if (collection == NULL) {
        return NULL;
    }
    
    char *input_copy = malloc((strlen(input) + 1) * sizeof(char));
    if (input_copy == NULL) {
        free_collection(collection);
        return NULL;
    }
    strcpy(input_copy, input);
    
    char *token = strtok(input_copy, " \t\n\r");
    while (token != NULL) {
        if (add_word(collection, token) < 0) {
            free(input_copy);
            free_collection(collection);
            return NULL;
        }
        token = strtok(NULL, " \t\n\r");
    }
    
    size_t total_len = 0;
    for (size_t i = 0; i < collection->count; i++) {
        total_len += strlen(collection->words[i]) + 1;
    }
    
    char *result = malloc((total_len + 1) * sizeof(char));
    if (result == NULL) {
        free(input_copy);
        free_collection(collection);
        return NULL;
    }
    
    result[0] = '\0';
    for (size_t i = 0; i < collection->count; i++) {
        strcat(result, collection->words[i]);
        if (i < collection->count - 1) {
            strcat(result, " ");
        }
    }
    
    free(input_copy);
    free_collection(collection);
    return result;
}

int main(void) {
    char input[1000];
    
    printf("Enter a string: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    char *result = remove_duplicate_words(input);
    if (result == NULL) {
        fprintf(stderr, "Error processing string\n");
        return EXIT_FAILURE;
    }
    
    printf("Result: %s\n", result);
    
    free(result);
    return EXIT_SUCCESS;
}