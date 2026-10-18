#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_WORD_LENGTH 100
#define MAX_UNIQUE_WORDS 1000
#define MAX_INPUT_LENGTH 4096

typedef struct {
    char *word;
    int count;
} WordCount;

static void free_word_counts(WordCount *word_counts, int count) {
    int i;

    if (word_counts == NULL) {
        return;
    }

    for (i = 0; i < count; i++) {
        free(word_counts[i].word);
        word_counts[i].word = NULL;
        word_counts[i].count = 0;
    }
}

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

    if (strings == NULL || num_strings <= 0) {
        return NULL;
    }

    for (i = 0; i < MAX_UNIQUE_WORDS; i++) {
        word_counts[i].word = NULL;
        word_counts[i].count = 0;
    }

    for (i = 0; i < num_strings; i++) {
        if (strings[i] == NULL) {
            continue;
        }

        string_copy = strndup(strings[i], MAX_INPUT_LENGTH);
        if (string_copy == NULL) {
            free_word_counts(word_counts, unique_words);
            return NULL;
        }

        token = strtok_r(string_copy, " \t\n\r", &saveptr);
        while (token != NULL) {
            int found = 0;

            for (j = 0; j < unique_words; j++) {
                if (strcmp(word_counts[j].word, token) == 0) {
                    if (word_counts[j].count < INT_MAX) {
                        word_counts[j].count++;
                    }
                    found = 1;
                    break;
                }
            }

            if (!found && unique_words < MAX_UNIQUE_WORDS) {
                word_counts[unique_words].word = strndup(token, MAX_WORD_LENGTH - 1);
                if (word_counts[unique_words].word == NULL) {
                    free(string_copy);
                    free_word_counts(word_counts, unique_words);
                    return NULL;
                }
                word_counts[unique_words].count = 1;
                unique_words++;
            }

            token = strtok_r(NULL, " \t\n\r", &saveptr);
        }

        free(string_copy);
        string_copy = NULL;
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

    result = strndup(word_counts[max_index].word, MAX_WORD_LENGTH - 1);

    free_word_counts(word_counts, unique_words);

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