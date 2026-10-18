#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    int position;
    char *word;
} Adverb;

int is_adverb(const char *word) {
    if (word == NULL) {
        return 0;
    }
    size_t len = strnlen(word, 1024);
    if (len == 1024) {
        return 0;
    }
    if (len > 2) {
        if (tolower((unsigned char)word[len - 2]) == 'l' && tolower((unsigned char)word[len - 1]) == 'y') {
            return 1;
        }
    }
    return 0;
}

Adverb *find_adverbs(const char *sentence, int *count) {
    if (sentence == NULL || count == NULL) {
        return NULL;
    }

    Adverb *results = NULL;
    *count = 0;
    size_t max_len = 65535;
    size_t len = strnlen(sentence, max_len);
    if (len == max_len) {
        return NULL;
    }
    
    if (len == 0) {
        return NULL;
    }

    size_t buffer_size = len + 1;
    char *buffer = malloc(buffer_size);
    if (buffer == NULL) {
        return NULL;
    }

    snprintf(buffer, buffer_size, "%s", sentence);

    int capacity = 10;
    results = malloc(capacity * sizeof(Adverb));
    if (results == NULL) {
        free(buffer);
        return NULL;
    }

    char *token = strtok(buffer, " ,.;!?\"'-");
    
    while (token != NULL) {
        /* Possible weaknesses found:
         * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
         */
        size_t token_len = strlen(token);
        
        if (is_adverb(token)) {
            if (*count >= capacity) {
                capacity *= 2;
                Adverb *temp = realloc(results, capacity * sizeof(Adverb));
                if (temp == NULL) {
                    for (int i = 0; i < *count; i++) {
                        free(results[i].word);
                    }
                    free(results);
                    free(buffer);
                    *count = 0;
                    return NULL;
                }
                results = temp;
            }

            size_t word_size = token_len + 1;
            results[*count].word = malloc(word_size);
            if (results[*count].word == NULL) {
                for (int i = 0; i < *count; i++) {
                    free(results[i].word);
                }
                free(results);
                free(buffer);
                *count = 0;
                return NULL;
            }
            
            snprintf(results[*count].word, word_size, "%s", token);
            results[*count].position = token - buffer;
            (*count)++;
        }
        
        token = strtok(NULL, " ,.;!?\"'-");
    }

    free(buffer);
    return results;
}

void free_adverbs(Adverb *adverbs, int count) {
    if (adverbs != NULL) {
        for (int i = 0; i < count; i++) {
            free(adverbs[i].word);
        }
        free(adverbs);
    }
}

int main() {
    const char *sentence = "She quickly ran to the store and happily bought the newly released book.";
    int count = 0;
    
    Adverb *adverbs = find_adverbs(sentence, &count);
    
    if (adverbs != NULL) {
        for (int i = 0; i < count; i++) {
            printf("Adverb: %s, Position: %d\n", adverbs[i].word, adverbs[i].position);
        }
        free_adverbs(adverbs, count);
    }
    
    return 0;
}