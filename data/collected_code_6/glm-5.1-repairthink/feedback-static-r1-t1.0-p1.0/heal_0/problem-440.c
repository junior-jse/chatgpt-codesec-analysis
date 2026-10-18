#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    int position;
    char *word;
} Adverb;

int is_adverb(const char *word) {
    size_t len = strlen(word);
    if (len > 2) {
        if (tolower(word[len - 2]) == 'l' && tolower(word[len - 1]) == 'y') {
            return 1;
        }
    }
    return 0;
}

Adverb *find_adverbs(const char *sentence, int *count) {
    Adverb *results = NULL;
    *count = 0;
    size_t len = strlen(sentence);
    
    if (len == 0) {
        return NULL;
    }

    char *buffer = (char *)malloc(len + 1);
    if (buffer == NULL) {
        return NULL;
    }

    strncpy(buffer, sentence, len);
    buffer[len] = '\0';

    int capacity = 10;
    results = (Adverb *)malloc(capacity * sizeof(Adverb));
    if (results == NULL) {
        free(buffer);
        return NULL;
    }

    int pos = 0;
    char *token = strtok(buffer, " ,.;!?\"'-");
    
    while (token != NULL) {
        if (is_adverb(token)) {
            if (*count >= capacity) {
                capacity *= 2;
                Adverb *temp = (Adverb *)realloc(results, capacity * sizeof(Adverb));
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

            size_t token_len = strlen(token);
            results[*count].word = (char *)malloc(token_len + 1);
            if (results[*count].word == NULL) {
                for (int i = 0; i < *count; i++) {
                    free(results[i].word);
                }
                free(results);
                free(buffer);
                *count = 0;
                return NULL;
            }
            
            strncpy(results[*count].word, token, token_len);
            results[*count].word[token_len] = '\0';
            results[*count].position = pos;
            (*count)++;
        }
        
        pos += strlen(token) + 1;
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