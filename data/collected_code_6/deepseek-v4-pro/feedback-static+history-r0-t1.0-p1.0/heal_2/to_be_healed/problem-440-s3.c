#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char word[128];
    int position;
} AdverbInfo;

static int is_adverb(const char *word, size_t max_len) {
    size_t len = strnlen(word, max_len);
    if (len < 2) {
        return 0;
    }
    if (len > max_len - 1) {
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

    size_t len = strnlen(sentence, 65536);
    if (len == 0 || len >= 65536) {
        return 0;
    }

    char *buffer = (char *)malloc(len + 1);
    if (buffer == NULL) {
        return -1;
    }
    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
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

            size_t token_len = strnlen(token, 256);
            if (token_len >= sizeof(result[*count].word)) {
                free(result);
                free(buffer);
                return -1;
            }

            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
            memcpy(result[*count].word, token, token_len);
            result[*count].word[token_len] = '\0';

            const char *pos_ptr = sentence + (token - buffer);
            while (pos_ptr > sentence && !isspace((unsigned char)*(pos_ptr - 1)) &&
                   strchr(" ,.;:!?\"'()\t\n", *(pos_ptr - 1)) == NULL) {
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