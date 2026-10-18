#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stddef.h>
#include <stdint.h>

typedef struct {
    int position;
    char *word;
} AdverbResult;

AdverbResult* find_adverbs(const char* sentence, int* count) {
    *count = 0;
    if (sentence == NULL) {
        return NULL;
    }

    size_t len = strnlen(sentence, SIZE_MAX - 1);
    char* buffer = malloc(len + 1);
    if (buffer == NULL) {
        return NULL;
    }
    snprintf(buffer, len + 1, "%s", sentence);

    AdverbResult* results = NULL;
    int capacity = 0;

    const char* delim = " \t\n\r.,;:!?()\"'";
    char* saveptr = NULL;
    char* token = strtok_r(buffer, delim, &saveptr);

    while (token != NULL) {
        size_t token_len = strnlen(token, 256);
        if (token_len > 2 && tolower((unsigned char)token[token_len - 2]) == 'l' && tolower((unsigned char)token[token_len - 1]) == 'y') {
            if (*count >= capacity) {
                capacity = capacity == 0 ? 8 : capacity * 2;
                AdverbResult* temp = realloc(results, capacity * sizeof(AdverbResult));
                if (temp == NULL) {
                    for (int i = 0; i < *count; i++) {
                        free(results[i].word);
                    }
                    free(buffer);
                    free(results);
                    *count = 0;
                    return NULL;
                }
                results = temp;
            }
            ptrdiff_t pos = token - buffer;
            results[*count].position = (int)pos;
            results[*count].word = malloc(token_len + 1);
            if (results[*count].word == NULL) {
                for (int i = 0; i < *count; i++) {
                    free(results[i].word);
                }
                free(buffer);
                free(results);
                *count = 0;
                return NULL;
            }
            snprintf(results[*count].word, token_len + 1, "%s", token);
            (*count)++;
        }
        token = strtok_r(NULL, delim, &saveptr);
    }

    free(buffer);
    return results;
}

int main(void) {
    const char* sentence = "She quickly ran to the store and happily bought a beautifully crafted gift.";
    int count = 0;
    AdverbResult* adverbs = find_adverbs(sentence, &count);

    if (adverbs != NULL) {
        for (int i = 0; i < count; i++) {
            printf("Adverb: %s, Position: %d\n", adverbs[i].word, adverbs[i].position);
        }
        for (int i = 0; i < count; i++) {
            free(adverbs[i].word);
        }
        free(adverbs);
    } else if (count == 0) {
        printf("No adverbs found.\n");
    } else {
        printf("Memory allocation failed.\n");
    }

    return 0;
}