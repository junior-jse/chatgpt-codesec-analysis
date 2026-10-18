#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_WORD_LENGTH 256
#define MAX_ADVERBS 100
#define MAX_SENTENCE_LENGTH 1024

typedef struct {
    char *word;
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

size_t find_adverbs(const char *sentence, size_t sentence_len, Adverb *adverbs, size_t max_adverbs) {
    size_t adverb_count = 0;
    size_t i = 0;
    
    if (sentence == NULL || adverbs == NULL || max_adverbs == 0) {
        return 0;
    }
    
    while (i < sentence_len && adverb_count < max_adverbs) {
        if (is_word_char((unsigned char)sentence[i])) {
            size_t word_start = i;
            size_t word_len = 0;
            
            while (i < sentence_len && is_word_char((unsigned char)sentence[i])) {
                i++;
                word_len++;
            }
            
            if (word_len > 0 && ends_with_ly(sentence + word_start, word_len)) {
                adverbs[adverb_count].word = malloc(word_len + 1);
                if (adverbs[adverb_count].word == NULL) {
                    for (size_t j = 0; j < adverb_count; j++) {
                        free(adverbs[j].word);
                    }
                    return 0;
                }
                memcpy(adverbs[adverb_count].word, sentence + word_start, word_len);
                adverbs[adverb_count].word[word_len] = '\0';
                adverbs[adverb_count].position = word_start;
                adverb_count++;
            }
        } else {
            i++;
        }
    }
    
    return adverb_count;
}

int main(void) {
    char sentence[MAX_SENTENCE_LENGTH];
    Adverb *adverbs = NULL;
    size_t adverb_count = 0;
    size_t sentence_len;
    
    printf("Enter a sentence: ");
    
    if (fgets(sentence, sizeof(sentence), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    sentence_len = strcspn(sentence, "\n");
    sentence[sentence_len] = '\0';
    
    adverbs = calloc(MAX_ADVERBS, sizeof(Adverb));
    if (adverbs == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }
    
    adverb_count = find_adverbs(sentence, sentence_len, adverbs, MAX_ADVERBS);
    
    if (adverb_count == 0) {
        printf("No adverbs found.\n");
    } else {
        printf("Found %zu adverb(s):\n", adverb_count);
        for (size_t i = 0; i < adverb_count; i++) {
            printf("  \"%s\" at position %zu\n", adverbs[i].word, adverbs[i].position);
        }
    }
    
    for (size_t i = 0; i < adverb_count; i++) {
        free(adverbs[i].word);
    }
    free(adverbs);
    adverbs = NULL;
    
    return EXIT_SUCCESS;
}