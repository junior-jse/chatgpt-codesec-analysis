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
    char (*words)[MAX_WORD_LEN] = NULL;
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char current_word[MAX_WORD_LEN];
    int word_count = 0;
    int word_len = 0;
    int i;
    int j;

    if (str == NULL) {
        return NULL;
    }

    words = calloc(MAX_WORDS, sizeof(*words));
    if (words == NULL) {
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
                        /* Possible weaknesses found:
                         * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
                         */
                        size_t len = strlen(current_word);
                        char *result = malloc(len + 1);
                        if (result == NULL) {
                            free(words);
                            return NULL;
                        }
                        if (len + 1 <= MAX_WORD_LEN) {
                            /* Possible weaknesses found:
                             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
                             */
                            memcpy(result, current_word, len + 1);
                        }
                        free(words);
                        return result;
                    }
                }

                if (word_count < MAX_WORDS) {
                    /* Possible weaknesses found:
                     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
                     */
                    size_t len = strlen(current_word);
                    if (len + 1 <= MAX_WORD_LEN) {
                        /* Possible weaknesses found:
                         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
                         */
                        memcpy(words[word_count], current_word, len + 1);
                    }
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
                /* Possible weaknesses found:
                 * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
                 */
                size_t len = strlen(current_word);
                char *result = malloc(len + 1);
                if (result == NULL) {
                    free(words);
                    return NULL;
                }
                if (len + 1 <= MAX_WORD_LEN) {
                    /* Possible weaknesses found:
                     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
                     */
                    memcpy(result, current_word, len + 1);
                }
                free(words);
                return result;
            }
        }
    }

    free(words);
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