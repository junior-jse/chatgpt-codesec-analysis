#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 256
#define MAX_WORDS 1000

char *find_first_repeated_word(const char *str) {
    char words[MAX_WORDS][MAX_WORD_LEN];
    int word_count = 0;
    char current_word[MAX_WORD_LEN];
    int word_len = 0;
    int i;
    int j;

    if (str == NULL) {
        return NULL;
    }

    for (i = 0; str[i] != '\0'; i++) {
        if (isalpha((unsigned char)str[i])) {
            if (word_len < MAX_WORD_LEN - 1) {
                current_word[word_len++] = tolower((unsigned char)str[i]);
            }
        } else {
            if (word_len > 0) {
                current_word[word_len] = '\0';

                for (j = 0; j < word_count; j++) {
                    if (strcmp(words[j], current_word) == 0) {
                        char *result = malloc(strlen(current_word) + 1);
                        if (result == NULL) {
                            return NULL;
                        }
                        strcpy(result, current_word);
                        return result;
                    }
                }

                if (word_count < MAX_WORDS) {
                    strcpy(words[word_count], current_word);
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
                char *result = malloc(strlen(current_word) + 1);
                if (result == NULL) {
                    return NULL;
                }
                strcpy(result, current_word);
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