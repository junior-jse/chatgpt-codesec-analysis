#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 256
#define MAX_WORDS 1000

char *find_first_repeated_word(const char *str) {
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char words[MAX_WORDS][MAX_WORD_LEN];
    int word_count = 0;
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char current_word[MAX_WORD_LEN];
    size_t word_len = 0;
    size_t i;
    int j;

    if (str == NULL) {
        return NULL;
    }

    for (i = 0; str[i] != '\0'; i++) {
        if (isalpha((unsigned char)str[i])) {
            if (word_len < MAX_WORD_LEN - 1) {
                current_word[word_len++] = (char)tolower((unsigned char)str[i]);
            }
        } else {
            if (word_len > 0) {
                current_word[word_len] = '\0';

                for (j = 0; j < word_count; j++) {
                    if (strcmp(words[j], current_word) == 0) {
                        char *result = malloc(word_len + 1);
                        if (result == NULL) {
                            return NULL;
                        }
                        /* Possible weaknesses found:
                         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
                         */
                        memcpy(result, current_word, word_len + 1);
                        return result;
                    }
                }

                if (word_count < MAX_WORDS) {
                    /* Possible weaknesses found:
                     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
                     */
                    memcpy(words[word_count], current_word, word_len + 1);
                    word_count++;
                }

                word_len = 0;
            }
        }
    }

    if (word_len > 0) {
        current_word[word_len] = '\0';

        for (j = 0; j < word_count; j++) {
            if (strcmp(words[j], current_word) == 0) {
                char *result = malloc(word_len + 1);
                if (result == NULL) {
                    return NULL;
                }
                /* Possible weaknesses found:
                 * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
                 */
                memcpy(result, current_word, word_len + 1);
                return result;
            }
        }
    }

    return NULL;
}

int main(void) {
    const char *test_string = "The quick brown fox jumps over the lazy dog. The dog was not amused.";
    char *repeated = find_first_repeated_word(test_string);

    if (repeated != NULL) {
        printf("First repeated word: %s\n", repeated);
        free(repeated);
    } else {
        printf("No repeated word found.\n");
    }

    return 0;
}