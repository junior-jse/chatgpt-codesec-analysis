#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>

#define HASH_SIZE 1009

typedef struct WordCount {
    char *word;
    int count;
    struct WordCount *next;
} WordCount;

typedef struct {
    WordCount *buckets[HASH_SIZE];
    int total_words;
} WordTable;

unsigned int hash_word(const char *word) {
    unsigned int hash = 5381;
    int c;
    while ((c = *word++)) {
        hash = ((hash << 5) + hash) + tolower((unsigned char)c);
    }
    return hash % HASH_SIZE;
}

WordTable *create_word_table(void) {
    WordTable *table = (WordTable *)malloc(sizeof(WordTable));
    if (!table) {
        return NULL;
    }
    for (int i = 0; i < HASH_SIZE; i++) {
        table->buckets[i] = NULL;
    }
    table->total_words = 0;
    return table;
}

void add_word_to_table(WordTable *table, const char *word, size_t len) {
    unsigned int index = hash_word(word);
    WordCount *entry = table->buckets[index];
    
    while (entry) {
        if (strcasecmp(entry->word, word) == 0) {
            entry->count++;
            return;
        }
        entry = entry->next;
    }
    
    entry = (WordCount *)malloc(sizeof(WordCount));
    if (!entry) {
        return;
    }
    entry->word = malloc(len + 1);
    if (!entry->word) {
        free(entry);
        return;
    }
    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(entry->word, word, len);
    entry->word[len] = '\0';
    entry->count = 1;
    entry->next = table->buckets[index];
    table->buckets[index] = entry;
    table->total_words++;
}

void free_word_table(WordTable *table) {
    if (!table) {
        return;
    }
    for (int i = 0; i < HASH_SIZE; i++) {
        WordCount *entry = table->buckets[i];
        while (entry) {
            WordCount *next = entry->next;
            free(entry->word);
            free(entry);
            entry = next;
        }
    }
    free(table);
}

void find_most_common_elements(const char *text) {
    if (!text) {
        fprintf(stderr, "Error: NULL input text\n");
        return;
    }
    
    WordTable *table = create_word_table();
    if (!table) {
        fprintf(stderr, "Error: Failed to allocate memory\n");
        return;
    }
    
    const char *p = text;
    size_t capacity = 64;
    char *current_word = malloc(capacity);
    if (!current_word) {
        free_word_table(table);
        fprintf(stderr, "Error: Memory allocation failed\n");
        return;
    }
    size_t word_len = 0;
    
    while (*p) {
        if (isalnum((unsigned char)*p) || *p == '_' || *p == '-') {
            if (word_len + 1 >= capacity) {
                capacity *= 2;
                char *tmp = realloc(current_word, capacity);
                if (!tmp) {
                    free(current_word);
                    free_word_table(table);
                    fprintf(stderr, "Error: Memory allocation failed\n");
                    return;
                }
                current_word = tmp;
            }
            current_word[word_len++] = (char)tolower((unsigned char)*p);
        } else {
            if (word_len > 0) {
                current_word[word_len] = '\0';
                add_word_to_table(table, current_word, word_len);
                word_len = 0;
            }
        }
        p++;
    }
    
    if (word_len > 0) {
        current_word[word_len] = '\0';
        add_word_to_table(table, current_word, word_len);
    }
    
    free(current_word);
    
    int max_count = 0;
    for (int i = 0; i < HASH_SIZE; i++) {
        WordCount *entry = table->buckets[i];
        while (entry) {
            if (entry->count > max_count) {
                max_count = entry->count;
            }
            entry = entry->next;
        }
    }
    
    if (max_count == 0) {
        printf("No words found in the text.\n");
        free_word_table(table);
        return;
    }
    
    printf("Most common element(s) with count %d:\n", max_count);
    for (int i = 0; i < HASH_SIZE; i++) {
        WordCount *entry = table->buckets[i];
        while (entry) {
            if (entry->count == max_count) {
                printf("  %d: %s\n", entry->count, entry->word);
            }
            entry = entry->next;
        }
    }
    
    free_word_table(table);
}

int main(int argc, char *argv[]) {
    if (argc > 1) {
        const char *text = argv[1];
        find_most_common_elements(text);
    } else {
        char *buffer = NULL;
        size_t buffer_size = 0;
        ssize_t bytes_read = getline(&buffer, &buffer_size, stdin);
        
        if (bytes_read == -1) {
            fprintf(stderr, "Error: Failed to read input\n");
            free(buffer);
            return EXIT_FAILURE;
        }
        
        if (bytes_read > 0 && buffer[bytes_read - 1] == '\n') {
            buffer[bytes_read - 1] = '\0';
        }
        
        find_most_common_elements(buffer);
        free(buffer);
    }
    
    return EXIT_SUCCESS;
}