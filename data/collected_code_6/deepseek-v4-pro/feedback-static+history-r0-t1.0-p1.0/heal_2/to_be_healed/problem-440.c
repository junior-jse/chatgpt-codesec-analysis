/* Repair history (previous round):
 * - Flawfinder strcpy was replaced by memcpy, which Flawfinder flags with the same CWE (CWE-120).
 * - Flawfinder strncpy was replaced by memcpy, which Flawfinder flags with the same CWE (CWE-120).
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_ADVERBS 100
#define MAX_WORD_LEN 50
#define MAX_POSITIONS 100

typedef struct {
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char word[MAX_WORD_LEN];
    int positions[MAX_POSITIONS];
    int position_count;
} AdverbInfo;

int is_adverb(const char *word) {
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t len = strlen(word);
    if (len >= 3 && strcmp(word + len - 2, "ly") == 0) {
        return 1;
    }
    if (strcmp(word, "well") == 0 || strcmp(word, "fast") == 0 ||
        strcmp(word, "hard") == 0 || strcmp(word, "high") == 0 ||
        strcmp(word, "late") == 0 || strcmp(word, "early") == 0 ||
        strcmp(word, "daily") == 0 || strcmp(word, "weekly") == 0 ||
        strcmp(word, "monthly") == 0 || strcmp(word, "yearly") == 0) {
        return 1;
    }
    return 0;
}

int find_adverbs(const char *sentence, AdverbInfo *adverbs, int max_adverbs) {
    if (sentence == NULL || adverbs == NULL || max_adverbs <= 0) {
        return 0;
    }

    int adverb_count = 0;
    int position = 0;
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t len = strlen(sentence);
    if (len >= (size_t)-1) {
        return 0;
    }
    char *temp = (char *)malloc(len + 1);
    if (temp == NULL) {
        return 0;
    }
    if (len > 0) {
        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(temp, sentence, len);
    }
    temp[len] = '\0';

    for (size_t i = 0; i < len; i++) {
        temp[i] = (char)tolower((unsigned char)temp[i]);
    }

    char *saveptr;
    char *token = strtok_r(temp, " ,.!?;:()\t\n\"'", &saveptr);
    while (token != NULL && adverb_count < max_adverbs) {
        if (is_adverb(token)) {
            int found = -1;
            for (int i = 0; i < adverb_count; i++) {
                if (strcmp(adverbs[i].word, token) == 0) {
                    found = i;
                    break;
                }
            }
            if (found == -1) {
                /* Possible weaknesses found:
                 * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
                 */
                size_t token_len = strlen(token);
                size_t copy_len = token_len < (size_t)(MAX_WORD_LEN - 1) 
                                  ? token_len : (size_t)(MAX_WORD_LEN - 1);
                /* Possible weaknesses found:
                 * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
                 */
                memcpy(adverbs[adverb_count].word, token, copy_len);
                adverbs[adverb_count].word[copy_len] = '\0';
                adverbs[adverb_count].positions[0] = position;
                adverbs[adverb_count].position_count = 1;
                adverb_count++;
            } else {
                if (adverbs[found].position_count < MAX_POSITIONS) {
                    adverbs[found].positions[adverbs[found].position_count] = position;
                    adverbs[found].position_count++;
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
    char *sentence = "He quickly ran fast and worked very hardly early daily well.";
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