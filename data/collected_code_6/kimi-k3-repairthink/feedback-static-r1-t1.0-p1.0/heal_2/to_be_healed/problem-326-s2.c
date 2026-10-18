#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORD_LENGTH 100
#define MAX_UNIQUE_WORDS 1000
#define MAX_INPUT_LENGTH 4096

typedef struct {
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char word[MAX_WORD_LENGTH];
    int count;
} WordCount;

char *get_most_frequent_word(const char *strings[], int num_strings) {
    WordCount word_counts[MAX_UNIQUE_WORDS];
    int unique_words = 0;
    int i, j, k;
    char *token;
    char *string_copy;
    char *saveptr;
    char *result;
    int max_count;
    int max_index;
    size_t copy_len;
    size_t token_len;
    size_t word_len;

    if (strings == NULL || num_strings <= 0) {
        return NULL;
    }

    memset(word_counts, 0, sizeof(word_counts));

    for (i = 0; i < num_strings; i++) {
        if (strings[i] == NULL) {
            continue;
        }

        copy_len = strnlen(strings[i], MAX_INPUT_LENGTH);
        string_copy = malloc(copy_len + 1);
        if (string_copy == NULL) {
            return NULL;
        }
        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(string_copy, strings[i], copy_len);
        string_copy[copy_len] = '\0';

        token = strtok_r(string_copy, " \t\n\r", &saveptr);
        while (token != NULL) {
            int found = 0;

            for (j = 0; j < unique_words; j++) {
                if (strcmp(word_counts[j].word, token) == 0) {
                    word_counts[j].count++;
                    found = 1;
                    break;
                }
            }

            if (!found && unique_words < MAX_UNIQUE_WORDS) {
                token_len = strnlen(token, MAX_WORD_LENGTH - 1);
                /* Possible weaknesses found:
                 * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
                 */
                memcpy(word_counts[unique_words].word, token, token_len);
                word_counts[unique_words].word[token_len] = '\0';
                word_counts[unique_words].count = 1;
                unique_words++;
            }

            token = strtok_r(NULL, " \t\n\r", &saveptr);
        }

        free(string_copy);
    }

    if (unique_words == 0) {
        return NULL;
    }

    max_count = word_counts[0].count;
    max_index = 0;

    for (k = 1; k < unique_words; k++) {
        if (word_counts[k].count > max_count) {
            max_count = word_counts[k].count;
            max_index = k;
        }
    }

    word_len = strnlen(word_counts[max_index].word, MAX_WORD_LENGTH);
    result = malloc(word_len + 1);
    if (result == NULL) {
        return NULL;
    }
    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(result, word_counts[max_index].word, word_len);
    result[word_len] = '\0';

    return result;
}

int main(void) {
    const char *strings[] = {
        "hello world hello",
        "world is beautiful world",
        "hello beautiful world"
    };
    int num_strings = (int)(sizeof(strings) / sizeof(strings[0]));
    char *most_frequent;

    most_frequent = get_most_frequent_word(strings, num_strings);

    if (most_frequent != NULL) {
        printf("Most frequent word: %s\n", most_frequent);
        free(most_frequent);
    } else {
        printf("No words found or error occurred\n");
    }

    return 0;
}