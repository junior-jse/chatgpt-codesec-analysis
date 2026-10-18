#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stddef.h>

typedef struct {
    int position;
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char word[256];
} AdverbResult;

AdverbResult* find_adverbs(const char* sentence, int* count) {
    *count = 0;
    if (sentence == NULL) {
        return NULL;
    }

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t len = strlen(sentence);
    char* buffer = malloc(len + 1);
    if (buffer == NULL) {
        return NULL;
    }
    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(buffer, sentence, len + 1);

    AdverbResult* results = NULL;
    int capacity = 0;

    const char* delim = " \t\n\r.,;:!?()\"'";
    char* saveptr = NULL;
    char* token = strtok_r(buffer, delim, &saveptr);

    while (token != NULL) {
        /* Possible weaknesses found:
         * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
         */
        size_t token_len = strlen(token);
        if (token_len > 2 && tolower((unsigned char)token[token_len - 2]) == 'l' && tolower((unsigned char)token[token_len - 1]) == 'y') {
            if (*count >= capacity) {
                capacity = capacity == 0 ? 8 : capacity * 2;
                AdverbResult* temp = realloc(results, capacity * sizeof(AdverbResult));
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
            snprintf(results[*count].word, sizeof(results[*count].word), "%s", token);
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