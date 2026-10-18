#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

typedef struct {
    char *word;
    size_t position;
} Adverb;

Adverb* find_adverbs(const char* sentence, size_t* out_count) {
    if (sentence == NULL || out_count == NULL) {
        return NULL;
    }

    *out_count = 0;
    size_t capacity = 10;
    Adverb* results = (Adverb*)malloc(capacity * sizeof(Adverb));
    if (results == NULL) {
        return NULL;
    }

    size_t index = 0;

    while (sentence[index] != '\0') {
        while (sentence[index] != '\0' && isspace((unsigned char)sentence[index])) {
            index++;
        }

        if (sentence[index] == '\0') {
            break;
        }

        size_t start_pos = index;

        size_t word_len = 0;
        while (sentence[index] != '\0' && !isspace((unsigned char)sentence[index])) {
            word_len++;
            index++;
        }

        if (word_len > 2 && sentence[start_pos + word_len - 2] == 'l' && sentence[start_pos + word_len - 1] == 'y') {
            if (*out_count >= capacity) {
                size_t new_capacity = capacity * 2;
                if (new_capacity <= capacity || new_capacity > SIZE_MAX / sizeof(Adverb)) {
                    for (size_t j = 0; j < *out_count; j++) {
                        free(results[j].word);
                    }
                    free(results);
                    return NULL;
                }
                Adverb* temp = (Adverb*)realloc(results, new_capacity * sizeof(Adverb));
                if (temp == NULL) {
                    for (size_t j = 0; j < *out_count; j++) {
                        free(results[j].word);
                    }
                    free(results);
                    return NULL;
                }
                results = temp;
                capacity = new_capacity;
            }

            /* Possible weaknesses found:
             *  alloc_size is assigned 'word_len+1' here.
             */
            size_t alloc_size = word_len + 1;
            /* Possible weaknesses found:
             *  Condition 'alloc_size<=word_len' is always false
             *  Condition 'alloc_size<=word_len' is always false [knownConditionTrueFalse]
             */
            if (alloc_size <= word_len) {
                for (size_t j = 0; j < *out_count; j++) {
                    free(results[j].word);
                }
                free(results);
                return NULL;
            }

            results[*out_count].word = (char*)malloc(alloc_size);
            if (results[*out_count].word == NULL) {
                for (size_t j = 0; j < *out_count; j++) {
                    free(results[j].word);
                }
                free(results);
                return NULL;
            }

            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
            memcpy(results[*out_count].word, sentence + start_pos, word_len);
            results[*out_count].word[word_len] = '\0';
            results[*out_count].position = start_pos;
            (*out_count)++;
        }
    }

    return results;
}

int main(void) {
    const char* sentence = "He quickly ran to the slowly moving car and happily drove away.";
    size_t count = 0;

    Adverb* adverbs = find_adverbs(sentence, &count);

    if (adverbs != NULL) {
        for (size_t i = 0; i < count; i++) {
            printf("Adverb: '%s' at position %zu\n", adverbs[i].word, adverbs[i].position);
        }
        for (size_t i = 0; i < count; i++) {
            free(adverbs[i].word);
        }
        free(adverbs);
    }

    return 0;
}