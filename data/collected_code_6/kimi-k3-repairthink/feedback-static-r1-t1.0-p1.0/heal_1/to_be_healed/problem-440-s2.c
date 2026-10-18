#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_WORD_LENGTH 256
#define MAX_ADVERBS 100

typedef struct {
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char word[MAX_WORD_LENGTH];
    size_t position;
} Adverb;

bool ends_with_ly(const char *word, size_t len) {
    if (len < 3) {
        return false;
    }
    return (word[len-2] == 'l' && word[len-1] == 'y');
}

bool is_word_char(int c) {
    return isalnum(c) || c == '\'' || c == '-';
}

size_t find_adverbs(const char *sentence, Adverb *adverbs, size_t max_adverbs) {
    size_t adverb_count = 0;
    size_t i = 0;
    size_t sentence_len;
    
    if (sentence == NULL || adverbs == NULL || max_adverbs == 0) {
        return 0;
    }
    
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    sentence_len = strlen(sentence);
    
    while (i < sentence_len && adverb_count < max_adverbs) {
        if (is_word_char((unsigned char)sentence[i])) {
            size_t word_start = i;
            size_t word_len = 0;
            
            while (i < sentence_len && is_word_char((unsigned char)sentence[i])) {
                i++;
                word_len++;
            }
            
            if (word_len > 0 && word_len < MAX_WORD_LENGTH) {
                if (ends_with_ly(sentence + word_start, word_len)) {
                    /* Possible weaknesses found:
                     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
                     */
                    memcpy(adverbs[adverb_count].word, sentence + word_start, word_len);
                    adverbs[adverb_count].word[word_len] = '\0';
                    adverbs[adverb_count].position = word_start;
                    adverb_count++;
                }
            }
        } else {
            i++;
        }
    }
    
    return adverb_count;
}

int main(void) {
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char sentence[1024];
    Adverb *adverbs = NULL;
    size_t adverb_count = 0;
    /* Possible weaknesses found:
     *  The scope of the variable 'i' can be reduced. [variableScope]
     */
    size_t i;
    
    printf("Enter a sentence: ");
    
    if (fgets(sentence, sizeof(sentence), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    sentence[strcspn(sentence, "\n")] = '\0';
    
    adverbs = malloc(MAX_ADVERBS * sizeof(Adverb));
    if (adverbs == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }
    
    adverb_count = find_adverbs(sentence, adverbs, MAX_ADVERBS);
    
    if (adverb_count == 0) {
        printf("No adverbs found.\n");
    } else {
        printf("Found %zu adverb(s):\n", adverb_count);
        for (i = 0; i < adverb_count; i++) {
            printf("  \"%s\" at position %zu\n", adverbs[i].word, adverbs[i].position);
        }
    }
    
    free(adverbs);
    adverbs = NULL;
    
    return EXIT_SUCCESS;
}