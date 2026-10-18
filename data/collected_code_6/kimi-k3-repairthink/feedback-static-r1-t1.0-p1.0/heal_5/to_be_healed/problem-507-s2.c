#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_WORD_LEN 1024

typedef struct {
    char **words;
    size_t count;
    size_t capacity;
} WordList;

static size_t bounded_strlen(const char *s, size_t max_len) {
    size_t len = 0;

    if (s == NULL) {
        return 0;
    }

    while (len < max_len && s[len] != '\0') {
        len++;
    }

    return len;
}

static int safe_copy(char *dest, size_t dest_size, const char *src, size_t src_len) {
    if (dest == NULL || src == NULL || dest_size == 0) {
        return -1;
    }

    if (src_len >= dest_size) {
        return -1;
    }

    if (src_len > 0) {
        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(dest, src, src_len);
    }
    dest[src_len] = '\0';

    return 0;
}

int init_word_list(WordList *list, size_t initial_capacity) {
    if (list == NULL || initial_capacity == 0) {
        return -1;
    }

    if (initial_capacity > SIZE_MAX / sizeof(char *)) {
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

    size_t word_len = bounded_strlen(word, MAX_WORD_LEN);
    if (word_len >= MAX_WORD_LEN) {
        return -1;
    }

    if (list->count >= list->capacity) {
        if (list->capacity > SIZE_MAX / 2) {
            return -1;
        }
        size_t new_capacity = list->capacity * 2;
        if (new_capacity > SIZE_MAX / sizeof(char *)) {
            return -1;
        }
        char **new_words = realloc(list->words, new_capacity * sizeof(char *));
        if (new_words == NULL) {
            return -1;
        }
        list->words = new_words;
        list->capacity = new_capacity;
    }

    if (word_len == SIZE_MAX) {
        return -1;
    }
    size_t buffer_size = word_len + 1;

    char *new_word = malloc(buffer_size);
    if (new_word == NULL) {
        return -1;
    }

    if (safe_copy(new_word, buffer_size, word, word_len) != 0) {
        free(new_word);
        return -1;
    }

    list->words[list->count] = new_word;
    list->count++;
    return 0;
}

void remove_word(WordList *list, const char *word) {
    if (list == NULL || word == NULL || list->count == 0) {
        return;
    }

    size_t write_index = 0;

    for (size_t i = 0; i < list->count; i++) {
        if (strncmp(list->words[i], word, MAX_WORD_LEN) != 0) {
            if (write_index != i) {
                list->words[write_index] = list->words[i];
            }
            write_index++;
        } else {
            free(list->words[i]);
            list->words[i] = NULL;
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
        fputs(list->words[i], stdout);
        if (i < list->count - 1) {
            putchar(' ');
        }
    }
    putchar('\n');
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