#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_ADVERBS 100
#define MAX_WORD_LEN 50
#define MAX_POSITIONS 100
#define MAX_SENTENCE_LEN 65536

typedef struct {
    char word[MAX_WORD_LEN];
    int positions[MAX_POSITIONS];
    int position_count;
} AdverbInfo;

static int is_adverb(const char *word, size_t word_len) {
    if (word_len >= 3 && word_len <= MAX_WORD_LEN - 1 && 
        word[word_len - 2] == 'l' && word[word_len - 1] == 'y') {
        return 1;
    }
    if ((word_len == 4 && memcmp(word, "well", 4) == 0) ||
        (word_len == 4 && memcmp(word, "fast", 4) == 0) ||
        (word_len == 4 && memcmp(word, "hard", 4) == 0) ||
        (word_len == 4 && memcmp(word, "high", 4) == 0) ||
        (word_len == 4 && memcmp(word, "late", 4) == 0) ||
        (word_len == 5 && memcmp(word, "early", 5) == 0) ||
        (word_len == 5 && memcmp(word, "daily", 5) == 0) ||
        (word_len == 6 && memcmp(word, "weekly", 6) == 0) ||
        (word_len == 7 && memcmp(word, "monthly", 7) == 0) ||
        (word_len == 6 && memcmp(word, "yearly", 6) == 0)) {
        return 1;
    }
    return 0;
}

static int find_in_adverbs(const AdverbInfo *adverbs, int count, const char *token, size_t token_len) {
    for (int i = 0; i < count; i++) {
        size_t word_len = strnlen(adverbs[i].word, MAX_WORD_LEN);
        if (word_len == token_len && word_len < MAX_WORD_LEN && 
            memcmp(adverbs[i].word, token, token_len) == 0) {
            return i;
        }
    }
    return -1;
}

static int copy_adverb_word(char *dest, const char *token, size_t token_len) {
    if (dest == NULL || token == NULL) {
        return 0;
    }
    if (token_len >= (size_t)MAX_WORD_LEN) {
        return 0;
    }
    memcpy(dest, token, token_len);
    dest[token_len] = '\0';
    return 1;
}

int find_adverbs(const char *sentence, AdverbInfo *adverbs, int max_adverbs) {
    if (sentence == NULL || adverbs == NULL || max_adverbs <= 0 || max_adverbs > MAX_ADVERBS) {
        return 0;
    }

    int adverb_count = 0;
    int position = 0;
    size_t len = strnlen(sentence, MAX_SENTENCE_LEN);
    if (len == 0 || len >= MAX_SENTENCE_LEN) {
        return 0;
    }

    char *temp = (char *)malloc(len + 1);
    if (temp == NULL) {
        return 0;
    }
    memcpy(temp, sentence, len);
    temp[len] = '\0';

    for (size_t i = 0; i < len; i++) {
        temp[i] = (char)tolower((unsigned char)temp[i]);
    }

    char *saveptr;
    char *token = strtok_r(temp, " ,.!?;:()\t\n\"'", &saveptr);
    while (token != NULL && adverb_count < max_adverbs) {
        size_t token_len = strnlen(token, MAX_WORD_LEN);
        if (token_len > 0 && token_len < MAX_WORD_LEN) {
            if (is_adverb(token, token_len)) {
                int found = find_in_adverbs(adverbs, adverb_count, token, token_len);
                if (found == -1) {
                    if (copy_adverb_word(adverbs[adverb_count].word, token, token_len)) {
                        adverbs[adverb_count].positions[0] = position;
                        adverbs[adverb_count].position_count = 1;
                        adverb_count++;
                    }
                } else {
                    if (adverbs[found].position_count < MAX_POSITIONS) {
                        adverbs[found].positions[adverbs[found].position_count] = position;
                        adverbs[found].position_count++;
                    }
                }
            }
        }
        position++;
        token = strtok_r(NULL, " ,.!?;:()\t\n\"'", &saveptr);
    }

    free(temp);
    return adverb_count;
}

int main(void) {
    char sentence[] = "He quickly ran fast and worked very hardly early daily well.";
    AdverbInfo adverbs[MAX_ADVERBS];
    int count = find_adverbs(sentence, adverbs, MAX_ADVERBS);

    printf("Sentence: %s\n", sentence);
    printf("Found %d adverb(s):\n", count);
    for (int i = 0; i < count; i++) {
        printf("  '%s' at position(s): ", adverbs[i].word);
        for (int j = 0; j < adverbs[i].position_count; j++) {
            printf("%d", adverbs[i].positions[j]);
            if (j < adverbs[i].position_count - 1) {
                printf(", ");
            }
        }
        printf("\n");
    }

    return 0;
}