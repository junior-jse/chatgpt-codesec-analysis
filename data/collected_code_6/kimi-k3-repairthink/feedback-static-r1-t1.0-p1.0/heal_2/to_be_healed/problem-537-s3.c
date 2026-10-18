#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 100
#define MAX_WORDS 1000
#define MAX_INPUT_LEN 1000

static char* duplicate_word(const char* word, size_t max_len) {
    size_t len = strnlen(word, max_len);
    char* copy = malloc(len + 1);
    if (copy != NULL) {
        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(copy, word, len);
        copy[len] = '\0';
    }
    return copy;
}

char* find_first_repeated_word(const char* str) {
    if (str == NULL) {
        return NULL;
    }

    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char (*words)[MAX_WORD_LEN] = calloc(MAX_WORDS, sizeof(*words));
    if (words == NULL) {
        return NULL;
    }

    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char current_word[MAX_WORD_LEN];
    int word_count = 0;
    size_t current_len = 0;
    char* result = NULL;

    for (size_t i = 0; ; i++) {
        unsigned char ch = (unsigned char)str[i];

        if (ch != '\0' && isalpha(ch)) {
            if (current_len < MAX_WORD_LEN - 1) {
                current_word[current_len++] = (char)tolower(ch);
            }
        } else {
            if (current_len > 0) {
                current_word[current_len] = '\0';

                for (int j = 0; j < word_count; j++) {
                    if (strncmp(words[j], current_word, MAX_WORD_LEN) == 0) {
                        result = duplicate_word(current_word, MAX_WORD_LEN);
                        goto cleanup;
                    }
                }

                if (word_count < MAX_WORDS) {
                    int written = snprintf(words[word_count], MAX_WORD_LEN, "%s", current_word);
                    if (written < 0 || written >= MAX_WORD_LEN) {
                        words[word_count][MAX_WORD_LEN - 1] = '\0';
                    }
                    word_count++;
                }

                current_len = 0;
            }

            if (ch == '\0') {
                break;
            }
        }
    }

cleanup:
    free(words);
    return result;
}

int main(void) {
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char input[MAX_INPUT_LEN];

    printf("Enter a string: ");
    if (fgets(input, sizeof(input), stdin) != NULL) {
        input[strcspn(input, "\n")] = '\0';

        char* result = find_first_repeated_word(input);

        if (result != NULL) {
            printf("First repeated word: %s\n", result);
            free(result);
        } else {
            printf("No repeated word found.\n");
        }
    }

    return 0;
}