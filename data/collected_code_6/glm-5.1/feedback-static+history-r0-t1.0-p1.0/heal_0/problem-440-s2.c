#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    int position;
    char word[256];
} AdverbResult;

AdverbResult* find_adverbs(const char* sentence, int* count) {
    *count = 0;
    if (sentence == NULL) {
        return NULL;
    }

    size_t len = strlen(sentence);
    char* buffer = (char*)malloc(len + 1);
    if (buffer == NULL) {
        return NULL;
    }
    strncpy(buffer, sentence, len + 1);

    AdverbResult* results = NULL;
    int capacity = 0;

    const char* delim = " \t\n\r.,;:!?()\"'";
    char* saveptr = NULL;
    char* token = strtok_r(buffer, delim, &saveptr);

    while (token != NULL) {
        size_t token_len = strlen(token);
        if (token_len > 2 && tolower((unsigned char)token[token_len - 2]) == 'l' && tolower((unsigned char)token[token_len - 1]) == 'y') {
            if (*count >= capacity) {
                capacity = capacity == 0 ? 8 : capacity * 2;
                AdverbResult* temp = (AdverbResult*)realloc(results, capacity * sizeof(AdverbResult));
                if (temp == NULL) {
                    free(buffer);
                    free(results);
                    *count = 0;
                    return NULL;
                }
                results = temp;
            }
            ptrdiff_t pos = token - buffer;
            results[*count].position = (int)pos;
            strncpy(results[*count].word, token, sizeof(results[*count].word) - 1);
            results[*count].word[sizeof(results[*count].word) - 1] = '\0';
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
        free(adverbs);
    } else if (count == 0) {
        printf("No adverbs found.\n");
    } else {
        printf("Memory allocation failed.\n");
    }

    return 0;
}