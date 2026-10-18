/* Repair history (previous round):
 * - Flawfinder strlen was removed from main() and now appears in wordset_contains().
 * - Flawfinder memcpy was replaced by strncpy, which Flawfinder flags with the same CWE (CWE-120).
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    char **words;
    size_t size;
    size_t capacity;
} WordSet;

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

int wordset_contains(WordSet *set, const char *word, size_t word_len) {
    for (size_t i = 0; i < set->size; i++) {
        /* Possible weaknesses found:
         * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
         */
        if (strlen(set->words[i]) == word_len && strncmp(set->words[i], word, word_len) == 0) {
            return 1;
        }
    }
    return 0;
}

int wordset_add(WordSet *set, const char *word, size_t word_len) {
    if (wordset_contains(set, word, word_len)) return 0;
    if (set->size >= set->capacity) {
        size_t new_cap = set->capacity * 2;
        char **new_words = realloc(set->words, new_cap * sizeof(char*));
        if (!new_words) return -1;
        set->words = new_words;
        set->capacity = new_cap;
    }
    set->words[set->size] = malloc(word_len + 1);
    if (!set->words[set->size]) return -1;
    /* Possible weaknesses found:
     * Flawfinder strncpy: Easily used incorrectly; doesn't always \0-terminate or check for invalid pointers [MS-banned] (CWE-120). (risk 1, buffer)
     */
    strncpy(set->words[set->size], word, word_len);
    set->words[set->size][word_len] = '\0';
    set->size++;
    return 1;
}

char* remove_duplicate_words(const char *input, size_t input_len) {
    if (!input) return NULL;
    WordSet *seen = wordset_create();
    if (!seen) return NULL;
    char *result = malloc(input_len + 1);
    if (!result) {
        wordset_destroy(seen);
        return NULL;
    }
    result[0] = '\0';
    size_t result_len = 0;
    size_t pos = 0;
    while (pos < input_len) {
        while (pos < input_len && isspace((unsigned char)input[pos])) pos++;
        if (pos >= input_len) break;
        size_t start = pos;
        while (pos < input_len && !isspace((unsigned char)input[pos])) pos++;
        size_t word_len = pos - start;
        int added = wordset_add(seen, input + start, word_len);
        if (added < 0) {
            wordset_destroy(seen);
            free(result);
            return NULL;
        }
        if (added > 0) {
            if (result_len > 0) {
                result[result_len++] = ' ';
            }
            /* Possible weaknesses found:
             * Flawfinder strncpy: Easily used incorrectly; doesn't always \0-terminate or check for invalid pointers [MS-banned] (CWE-120). (risk 1, buffer)
             */
            strncpy(result + result_len, input + start, word_len);
            result_len += word_len;
            result[result_len] = '\0';
        }
    }
    wordset_destroy(seen);
    return result;
}

int main(void) {
    const char test[] = "hello world hello there world world";
    size_t test_len = sizeof(test) - 1;
    char *result = remove_duplicate_words(test, test_len);
    if (result) {
        printf("%s\n", result);
        free(result);
    }
    return 0;
}