#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#define MAX_INPUT_LENGTH (16u * 1024u * 1024u)

typedef struct {
    char **words;
    size_t size;
    size_t capacity;
} WordSet;

static size_t bounded_strlen(const char *s, size_t max_len) {
    size_t n = 0;
    if (!s) return 0;
    while (n < max_len && s[n] != '\0') {
        n++;
    }
    return n;
}

WordSet* wordset_create(void) {
    WordSet *set = malloc(sizeof(WordSet));
    if (!set) return NULL;
    set->capacity = 16;
    set->size = 0;
    set->words = malloc(set->capacity * sizeof(char *));
    if (!set->words) {
        free(set);
        return NULL;
    }
    return set;
}

void wordset_destroy(WordSet *set) {
    if (!set) return;
    for (size_t i = 0; i < set->size; i++) {
        free(set->words[i]);
    }
    free(set->words);
    free(set);
}

int wordset_contains(WordSet *set, const char *word) {
    if (!set || !word) return 0;
    for (size_t i = 0; i < set->size; i++) {
        if (strcmp(set->words[i], word) == 0) return 1;
    }
    return 0;
}

int wordset_add(WordSet *set, const char *word) {
    if (!set || !word) return -1;
    if (wordset_contains(set, word)) return 0;
    if (set->size >= set->capacity) {
        if (set->capacity > SIZE_MAX / (2u * sizeof(char *))) return -1;
        size_t new_cap = set->capacity * 2u;
        char **new_words = realloc(set->words, new_cap * sizeof(char *));
        if (!new_words) return -1;
        set->words = new_words;
        set->capacity = new_cap;
    }
    size_t word_len = bounded_strlen(word, MAX_INPUT_LENGTH);
    if (word_len >= MAX_INPUT_LENGTH) return -1;
    char *copy = malloc(word_len + 1u);
    if (!copy) return -1;
    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(copy, word, word_len);
    copy[word_len] = '\0';
    set->words[set->size] = copy;
    set->size++;
    return 1;
}

char* remove_duplicate_words(const char *input) {
    if (!input) return NULL;
    size_t len = bounded_strlen(input, MAX_INPUT_LENGTH);
    if (len >= MAX_INPUT_LENGTH) return NULL;
    WordSet *seen = wordset_create();
    if (!seen) return NULL;
    size_t result_capacity = len + 1u;
    char *result = malloc(result_capacity);
    if (!result) {
        wordset_destroy(seen);
        return NULL;
    }
    result[0] = '\0';
    size_t result_len = 0;
    size_t pos = 0;
    while (pos < len) {
        while (pos < len && isspace((unsigned char)input[pos])) pos++;
        if (pos >= len) break;
        size_t start = pos;
        while (pos < len && !isspace((unsigned char)input[pos])) pos++;
        size_t word_len = pos - start;
        char *word = malloc(word_len + 1u);
        if (!word) {
            wordset_destroy(seen);
            free(result);
            return NULL;
        }
        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(word, input + start, word_len);
        word[word_len] = '\0';
        int added = wordset_add(seen, word);
        if (added < 0) {
            free(word);
            wordset_destroy(seen);
            free(result);
            return NULL;
        }
        if (added > 0) {
            size_t separator = (result_len > 0) ? 1u : 0u;
            size_t needed = word_len + separator + 1u;
            if (needed > result_capacity - result_len) {
                free(word);
                wordset_destroy(seen);
                free(result);
                return NULL;
            }
            if (separator) {
                result[result_len++] = ' ';
            }
            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
            memcpy(result + result_len, word, word_len);
            result_len += word_len;
            result[result_len] = '\0';
        }
        free(word);
    }
    wordset_destroy(seen);
    return result;
}

int main(void) {
    const char *test = "hello world hello there world world";
    char *result = remove_duplicate_words(test);
    if (result) {
        printf("%s\n", result);
        free(result);
    } else {
        fprintf(stderr, "remove_duplicate_words failed\n");
        return 1;
    }
    return 0;
}