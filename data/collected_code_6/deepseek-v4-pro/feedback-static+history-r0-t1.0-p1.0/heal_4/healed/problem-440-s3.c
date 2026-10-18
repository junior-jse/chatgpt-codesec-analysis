#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    char word[128];
    int position;
} AdverbInfo;

static int is_adverb(const char *word, size_t max_len) {
    size_t len = strnlen(word, max_len);
    if (len < 2) {
        return 0;
    }
    if (len >= max_len) {
        return 0;
    }
    return strncmp(word + len - 2, "ly", 2) == 0;
}

static int find_all_adverbs(const char *sentence, AdverbInfo **adverbs, int *count) {
    if (sentence == NULL || adverbs == NULL || count == NULL) {
        return -1;
    }

    *adverbs = NULL;
    *count = 0;

    size_t len = strnlen(sentence, 65535);
    if (len == 0 || len >= 65535) {
        return 0;
    }

    char *buffer = (char *)malloc(len + 1);
    if (buffer == NULL) {
        return -1;
    }

    memcpy(buffer, sentence, len);
    buffer[len] = '\0';

    int capacity = 0;
    AdverbInfo *result = NULL;

    char *saveptr = NULL;
    char *token = strtok_r(buffer, " ,.;:!?\"'()\t\n", &saveptr);
    while (token != NULL) {
        if (is_adverb(token, 128)) {
            if (*count >= capacity) {
                int new_capacity = capacity == 0 ? 4 : capacity * 2;
                if (new_capacity > 1024) {
                    free(result);
                    free(buffer);
                    return -1;
                }
                AdverbInfo *new_result = (AdverbInfo *)realloc(result, (size_t)new_capacity * sizeof(AdverbInfo));
                if (new_result == NULL) {
                    free(result);
                    free(buffer);
                    return -1;
                }
                result = new_result;
                capacity = new_capacity;
            }

            size_t token_len = strnlen(token, 127);
            if (token_len >= sizeof(result[*count].word)) {
                free(result);
                free(buffer);
                return -1;
            }

            memcpy(result[*count].word, token, token_len);
            result[*count].word[token_len] = '\0';

            const char *pos_ptr = sentence + (token - buffer);
            while (pos_ptr > sentence) {
                unsigned char prev_char = (unsigned char)*(pos_ptr - 1);
                if (isspace(prev_char) || strchr(" ,.;:!?\"'()\t\n", prev_char) != NULL) {
                    break;
                }
                pos_ptr--;
            }
            result[*count].position = (int)(pos_ptr - sentence);

            (*count)++;
        }
        token = strtok_r(NULL, " ,.;:!?\"'()\t\n", &saveptr);
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