#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    char *word;
    int position;
} AdverbInfo;

static int is_adverb(const char *word) {
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t len = strlen(word);
    if (len < 2) {
        return 0;
    }
    return strcmp(word + len - 2, "ly") == 0;
}

static void free_adverbs(AdverbInfo *adverbs, int count) {
    if (adverbs != NULL) {
        for (int i = 0; i < count; i++) {
            free(adverbs[i].word);
        }
        free(adverbs);
    }
}

static int find_all_adverbs(const char *sentence, AdverbInfo **adverbs, int *count) {
    if (sentence == NULL || adverbs == NULL || count == NULL) {
        return -1;
    }

    *adverbs = NULL;
    *count = 0;

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t len = strlen(sentence);
    if (len == 0) {
        return 0;
    }

    char *buffer = strdup(sentence);
    if (buffer == NULL) {
        return -1;
    }

    int capacity = 0;
    AdverbInfo *result = NULL;

    const char *delimiters = " ,.;:!?\"'()\t\n";
    char *token = strtok(buffer, delimiters);
    while (token != NULL) {
        if (is_adverb(token)) {
            if (*count >= capacity) {
                int new_capacity = (capacity == 0) ? 4 : capacity * 2;
                AdverbInfo *new_result = realloc(result, (size_t)new_capacity * sizeof(AdverbInfo));
                if (new_result == NULL) {
                    free_adverbs(result, *count);
                    free(buffer);
                    return -1;
                }
                result = new_result;
                capacity = new_capacity;
            }

            result[*count].word = strdup(token);
            if (result[*count].word == NULL) {
                free_adverbs(result, *count);
                free(buffer);
                return -1;
            }

            const char *pos_ptr = sentence + (token - buffer);
            while (pos_ptr > sentence && !strchr(delimiters, *(pos_ptr - 1))) {
                pos_ptr--;
            }
            result[*count].position = (int)(pos_ptr - sentence);

            (*count)++;
        }
        token = strtok(NULL, delimiters);
    }

    free(buffer);

    if (*count == 0) {
        free(result);
        *adverbs = NULL;
    } else {
        *adverbs = result;
    }

    return 0;
}

int main(void) {
    const char *sentence = "The cat quickly ran very slowly and happily through the garden.";
    AdverbInfo *adverbs = NULL;
    int count = 0;

    int status = find_all_adverbs(sentence, &adverbs, &count);
    if (status != 0) {
        fprintf(stderr, "Error processing sentence\n");
        return EXIT_FAILURE;
    }

    printf("Adverbs found: %d\n", count);
    for (int i = 0; i < count; i++) {
        printf("Position %d: %s\n", adverbs[i].position, adverbs[i].word);
    }

    free_adverbs(adverbs, count);
    return EXIT_SUCCESS;
}