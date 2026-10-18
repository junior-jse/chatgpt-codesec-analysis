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
    const char *end = memchr(word, '\0', 1024);
    if (end == NULL) {
        return 0;
    }
    size_t len = end - word;
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
    const char *end = memchr(sentence, '\0', max_len);
    if (end == NULL) {
        return NULL;
    }
    size_t len = end - sentence;
    
    if (len == 0) {
        return NULL;
    }

    char *buffer = malloc(len + 1);
    if (buffer == NULL) {
        return NULL;
    }

    memcpy(buffer, sentence, len);
    buffer[len] = '\0';

    int capacity = 10;
    results = malloc(capacity * sizeof(Adverb));
    if (results == NULL) {
        free(buffer);
        return NULL;
    }

    int pos = 0;
    char *token = strtok(buffer, " ,.;!?\"'-");
    
    while (token != NULL) {
        end = memchr(token, '\0', len + 1);
        if (end == NULL) {
            break;
        }
        size_t token_len = end - token;
        
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

            results[*count].word = malloc(token_len + 1);
            if (results[*count].word == NULL) {
                for (int i = 0; i < *count; i++) {
                    free(results[i].word);
                }
                free(results);
                free(buffer);
                *count = 0;
                return NULL;
            }
            
            memcpy(results[*count].word, token, token_len);
            results[*count].word[token_len] = '\0';
            results[*count].position = pos;
            (*count)++;
        }
        
        pos += token_len + 1;
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