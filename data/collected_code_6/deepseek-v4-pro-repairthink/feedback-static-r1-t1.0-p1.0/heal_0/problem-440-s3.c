#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    char word[128];
    int position;
} AdverbInfo;

static int is_adverb(const char *word) {
    size_t len = strlen(word);
    if (len < 2) {
        return 0;
    }
    return strcmp(word + len - 2, "ly") == 0;
}

static int find_all_adverbs(const char *sentence, AdverbInfo **adverbs, int *count) {
    if (sentence == NULL || adverbs == NULL || count == NULL) {
        return -1;
    }

    *adverbs = NULL;
    *count = 0;

    size_t len = strlen(sentence);
    if (len == 0) {
        return 0;
    }

    char *buffer = (char *)malloc(len + 1);
    if (buffer == NULL) {
        return -1;
    }
    memcpy(buffer, sentence, len + 1);

    int capacity = 0;
    AdverbInfo *result = NULL;

    char *token = strtok(buffer, " ,.;:!?\"'()\t\n");
    while (token != NULL) {
        if (is_adverb(token)) {
            if (*count >= capacity) {
                int new_capacity = capacity == 0 ? 4 : capacity * 2;
                AdverbInfo *new_result = (AdverbInfo *)realloc(result, (size_t)new_capacity * sizeof(AdverbInfo));
                if (new_result == NULL) {
                    free(result);
                    free(buffer);
                    return -1;
                }
                result = new_result;
                capacity = new_capacity;
            }

            size_t token_len = strlen(token);
            if (token_len >= sizeof(result[*count].word)) {
                free(result);
                free(buffer);
                return -1;
            }

            memcpy(result[*count].word, token, token_len + 1);

            const char *pos_ptr = sentence + (token - buffer);
            while (pos_ptr > sentence && !isspace((unsigned char)*(pos_ptr - 1)) &&
                   strchr(" ,.;:!?\"'()\t\n", *(pos_ptr - 1)) == NULL) {
                pos_ptr--;
            }
            result[*count].position = (int)(pos_ptr - sentence);

            (*count)++;
        }
        token = strtok(NULL, " ,.;:!?\"'()\t\n");
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

    free(adverbs);
    return EXIT_SUCCESS;
}