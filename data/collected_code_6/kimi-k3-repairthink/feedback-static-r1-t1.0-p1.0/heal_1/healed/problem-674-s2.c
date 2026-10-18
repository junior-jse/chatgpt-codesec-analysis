#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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
    set->words = malloc(set->capacity * sizeof(char*));
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
        size_t new_cap = set->capacity * 2;
        char **new_words = realloc(set->words, new_cap * sizeof(char*));
        if (!new_words) return -1;
        set->words = new_words;
        set->capacity = new_cap;
    }
    size_t word_len = bounded_strlen(word, MAX_INPUT_LENGTH);
    if (word_len >= MAX_INPUT_LENGTH) return -1;
    set->words[set->size] = malloc(word_len + 1);
    if (!set->words[set->size]) return -1;
    snprintf(set->words[set->size], word_len + 1, "%s", word);
    set->size++;
    return 1;
}

char* remove_duplicate_words(const char *input) {
    if (!input) return NULL;
    size_t len = bounded_strlen(input, MAX_INPUT_LENGTH);
    if (len >= MAX_INPUT_LENGTH) return NULL;
    WordSet *seen = wordset_create();
    if (!seen) return NULL;
    char *result = malloc(len + 1);
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
        char *word = malloc(word_len + 1);
        if (!word) {
            wordset_destroy(seen);
            free(result);
            return NULL;
        }
        snprintf(word, word_len + 1, "%.*s", (int)word_len, input + start);
        int added = wordset_add(seen, word);
        if (added < 0) {
            free(word);
            wordset_destroy(seen);
            free(result);
            return NULL;
        }
        if (added > 0) {
            size_t separator = (result_len > 0) ? 1u : 0u;
            if (result_len + separator + word_len > len) {
                free(word);
                wordset_destroy(seen);
                free(result);
                return NULL;
            }
            if (separator) {
                result[result_len++] = ' ';
            }
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
    }
    return 0;
}