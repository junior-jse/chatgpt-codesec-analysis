#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    char *word;
    int count;
} WordCount;

char *extract_word(const char **text) {
    while (**text && !isalpha((unsigned char)**text)) {
        (*text)++;
    }
    if (!**text) {
        return NULL;
    }
    const char *start = *text;
    while (**text && isalpha((unsigned char)**text)) {
        (*text)++;
    }
    size_t len = *text - start;
    char *word = malloc(len + 1);
    if (!word) {
        return NULL;
    }
    memcpy(word, start, len);
    word[len] = '\0';
    for (size_t i = 0; i < len; i++) {
        word[i] = tolower((unsigned char)word[i]);
    }
    return word;
}

WordCount *find_most_common(const char *text, int *result_size) {
    if (!text || !result_size) {
        return NULL;
    }
    int capacity = 16;
    int size = 0;
    WordCount *counts = malloc(capacity * sizeof(WordCount));
    if (!counts) {
        return NULL;
    }
    const char *ptr = text;
    char *word;
    while ((word = extract_word(&ptr)) != NULL) {
        int found = 0;
        for (int i = 0; i < size; i++) {
            if (strcmp(counts[i].word, word) == 0) {
                counts[i].count++;
                found = 1;
                free(word);
                word = NULL;
                break;
            }
        }
        if (!found) {
            if (size == capacity) {
                int new_capacity = capacity * 2;
                WordCount *new_counts = realloc(counts, new_capacity * sizeof(WordCount));
                if (!new_counts) {
                    free(word);
                    for (int i = 0; i < size; i++) free(counts[i].word);
                    free(counts);
                    return NULL;
                }
                counts = new_counts;
                capacity = new_capacity;
            }
            counts[size].word = word;
            counts[size].count = 1;
            size++;
        }
    }
    *result_size = size;
    return counts;
}

int main(void) {
    const char *text = "Hello world! The world is a beautiful place. Hello again, world.";
    int count_size = 0;
    WordCount *counts = find_most_common(text, &count_size);
    if (!counts) {
        return 1;
    }
    for (int i = 0; i < count_size; i++) {
        printf("%s: %d\n", counts[i].word, counts[i].count);
    }
    for (int i = 0; i < count_size; i++) {
        free(counts[i].word);
    }
    free(counts);
    return 0;
}