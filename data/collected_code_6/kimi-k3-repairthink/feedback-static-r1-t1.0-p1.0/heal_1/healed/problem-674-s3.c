#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORDS 1000
#define MAX_WORD_LEN 100
#define MAX_INPUT_LEN 8192

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

    if (initial_capacity == 0) {
        initial_capacity = 1;
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

    size_t word_len = strnlen(word, MAX_INPUT_LEN);
    char *word_copy = malloc((word_len + 1) * sizeof(char));
    if (word_copy == NULL) {
        return -1;
    }
    memcpy(word_copy, word, word_len);
    word_copy[word_len] = '\0';

    collection->words[collection->count] = word_copy;
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

    size_t input_len = strnlen(input, MAX_INPUT_LEN);
    char *input_copy = malloc((input_len + 1) * sizeof(char));
    if (input_copy == NULL) {
        free_collection(collection);
        return NULL;
    }
    memcpy(input_copy, input, input_len);
    input_copy[input_len] = '\0';

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
        total_len += strnlen(collection->words[i], MAX_INPUT_LEN) + 1;
    }

    char *result = malloc((total_len + 1) * sizeof(char));
    if (result == NULL) {
        free(input_copy);
        free_collection(collection);
        return NULL;
    }

    size_t pos = 0;
    for (size_t i = 0; i < collection->count; i++) {
        size_t word_len = strnlen(collection->words[i], MAX_INPUT_LEN);
        memcpy(result + pos, collection->words[i], word_len);
        pos += word_len;
        if (i < collection->count - 1) {
            result[pos++] = ' ';
        }
    }
    result[pos] = '\0';

    free(input_copy);
    free_collection(collection);
    return result;
}

int main(void) {
    char *input = malloc(MAX_INPUT_LEN * sizeof(char));
    if (input == NULL) {
        fprintf(stderr, "Error allocating memory\n");
        return EXIT_FAILURE;
    }

    printf("Enter a string: ");
    if (fgets(input, MAX_INPUT_LEN, stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        free(input);
        return EXIT_FAILURE;
    }

    input[strcspn(input, "\n")] = '\0';

    char *result = remove_duplicate_words(input);
    free(input);
    if (result == NULL) {
        fprintf(stderr, "Error processing string\n");
        return EXIT_FAILURE;
    }

    printf("Result: %s\n", result);

    free(result);
    return EXIT_SUCCESS;
}