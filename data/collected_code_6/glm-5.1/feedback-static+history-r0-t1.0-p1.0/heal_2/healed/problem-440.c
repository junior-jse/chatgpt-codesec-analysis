#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    int position;
    char *word;
} Adverb;

int is_adverb(const char *word, size_t len) {
    if (len > 2) {
        if (tolower((unsigned char)word[len - 2]) == 'l' && tolower((unsigned char)word[len - 1]) == 'y') {
            return 1;
        }
    }
    return 0;
}

Adverb *find_adverbs(const char *sentence, int *count) {
    Adverb *results = NULL;
    *count = 0;
    size_t len = strnlen(sentence, SIZE_MAX);
    
    if (len == 0) {
        return NULL;
    }

    char *buffer = (char *)malloc(len + 1);
    if (buffer == NULL) {
        return NULL;
    }

    if (len + 1 > len) {
        memcpy(buffer, sentence, len + 1);
    } else {
        buffer[0] = '\0';
    }

    int capacity = 10;
    results = (Adverb *)malloc(capacity * sizeof(Adverb));
    if (results == NULL) {
        free(buffer);
        return NULL;
    }

    int pos = 0;
    char *token = strtok(buffer, " ,.;!?\"'-");
    
    while (token != NULL) {
        size_t token_len = strnlen(token, len - (token - buffer));
        if (is_adverb(token, token_len)) {
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

            if (token_len + 1 == 0) {
                token_len = 0;
            }
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
            
            if (token_len + 1 > token_len) {
                memcpy(results[*count].word, token, token_len + 1);
            } else {
                results[*count].word[0] = '\0';
            }
            
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