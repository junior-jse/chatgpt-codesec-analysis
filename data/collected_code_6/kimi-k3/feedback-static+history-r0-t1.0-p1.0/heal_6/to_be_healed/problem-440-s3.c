/* Repair history (previous round):
 * - Flawfinder strncpy was replaced by memcpy, which Flawfinder flags with the same CWE (CWE-120).
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#define MAX_WORD_LENGTH 256
#define MAX_ADVERBS 100

typedef struct {
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char word[MAX_WORD_LENGTH];
    size_t position;
} AdverbInfo;

int is_adverb(const char *word, size_t len) {
    if (word == NULL || len < 2) {
        return 0;
    }
    return (word[len - 2] == 'l' && word[len - 1] == 'y');
}

int find_adverbs(const char *sentence, size_t sentence_len, AdverbInfo *adverbs, size_t max_adverbs) {
    if (sentence == NULL || adverbs == NULL || max_adverbs == 0) {
        return -1;
    }

    size_t count = 0;
    size_t pos = 0;
    size_t word_start = 0;
    size_t word_len = 0;

    /* Possible weaknesses found:
     *  outer condition: count<max_adverbs
     */
    while (pos <= sentence_len && count < max_adverbs) {
        if (pos == sentence_len || isspace((unsigned char)sentence[pos])) {
            if (word_len > 0) {
                if (word_len >= MAX_WORD_LENGTH) {
                    return -1;
                }
                
                if (is_adverb(sentence + word_start, word_len)) {
                    /* Possible weaknesses found:
                     *  opposite inner condition: count>=max_adverbs
                     *  Opposite inner 'if' condition leads to a dead code block. [oppositeInnerCondition]
                     */
                    if (count >= max_adverbs) {
                        return -1;
                    }
                    
                    if (word_len < MAX_WORD_LENGTH) {
                        /* Possible weaknesses found:
                         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
                         */
                        memcpy(adverbs[count].word, sentence + word_start, word_len);
                        adverbs[count].word[word_len] = '\0';
                        adverbs[count].position = word_start;
                        count++;
                    } else {
                        return -1;
                    }
                }
            }
            word_start = pos + 1;
            word_len = 0;
        } else {
            word_len++;
        }
        pos++;
    }

    if (count > (size_t)INT32_MAX) {
        return -1;
    }

    return (int)count;
}

int main(void) {
    const char sentence[] = "She quickly ran to the store and carefully picked the items. He slowly walked home.";
    const size_t sentence_len = sizeof(sentence) - 1;
    AdverbInfo *adverbs = malloc(MAX_ADVERBS * sizeof(AdverbInfo));
    
    if (adverbs == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    int count = find_adverbs(sentence, sentence_len, adverbs, MAX_ADVERBS);
    
    if (count < 0) {
        fprintf(stderr, "Error processing sentence\n");
        free(adverbs);
        return EXIT_FAILURE;
    }

    printf("Found %d adverb(s):\n", count);
    for (int i = 0; i < count; i++) {
        printf("Adverb: %s, Position: %zu\n", adverbs[i].word, adverbs[i].position);
    }

    free(adverbs);
    return EXIT_SUCCESS;
}