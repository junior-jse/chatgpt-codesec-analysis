#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char word[256];
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

    /* Possible weaknesses found:
     *  Variable 'ptr' is assigned a value that is never used. [unreadVariable]
     */
    const char* ptr = sentence;
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

        if (word_len > 2 && word_len < 256 && sentence[start_pos + word_len - 2] == 'l' && sentence[start_pos + word_len - 1] == 'y') {
            if (*out_count >= capacity) {
                size_t new_capacity = capacity * 2;
                Adverb* temp = (Adverb*)realloc(results, new_capacity * sizeof(Adverb));
                if (temp == NULL) {
                    free(results);
                    return NULL;
                }
                results = temp;
                capacity = new_capacity;
            }

            size_t copy_len = word_len < 255 ? word_len : 255;
            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
            memcpy(results[*out_count].word, &sentence[start_pos], copy_len);
            results[*out_count].word[copy_len] = '\0';
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
        free(adverbs);
    }

    return 0;
}