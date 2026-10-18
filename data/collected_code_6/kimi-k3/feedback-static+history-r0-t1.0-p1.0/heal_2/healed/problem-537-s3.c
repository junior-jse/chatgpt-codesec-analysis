#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 100
#define MAX_WORDS 1000
#define MAX_INPUT_LEN 1000

char* find_first_repeated_word(const char* str) {
    if (str == NULL) {
        return NULL;
    }

    char words[MAX_WORDS][MAX_WORD_LEN];
    int word_count = 0;
    char current_word[MAX_WORD_LEN];
    size_t current_len = 0;
    size_t i = 0;

    while (str[i] != '\0') {
        if (isalpha((unsigned char)str[i])) {
            if (current_len < MAX_WORD_LEN - 1) {
                current_word[current_len++] = (char)tolower((unsigned char)str[i]);
            }
        } else {
            if (current_len > 0) {
                current_word[current_len] = '\0';

                for (int j = 0; j < word_count; j++) {
                    if (strncmp(words[j], current_word, MAX_WORD_LEN) == 0) {
                        size_t len = strnlen(current_word, MAX_WORD_LEN);
                        char* result = malloc(len + 1);
                        if (result != NULL) {
                            strncpy(result, current_word, len);
                            result[len] = '\0';
                        }
                        return result;
                    }
                }

                if (word_count < MAX_WORDS) {
                    strncpy(words[word_count], current_word, MAX_WORD_LEN - 1);
                    words[word_count][MAX_WORD_LEN - 1] = '\0';
                    word_count++;
                }

                current_len = 0;
            }
        }
        i++;
    }

    if (current_len > 0) {
        current_word[current_len] = '\0';

        for (int j = 0; j < word_count; j++) {
            if (strncmp(words[j], current_word, MAX_WORD_LEN) == 0) {
                size_t len = strnlen(current_word, MAX_WORD_LEN);
                char* result = malloc(len + 1);
                if (result != NULL) {
                    strncpy(result, current_word, len);
                    result[len] = '\0';
                }
                return result;
            }
        }
    }

    return NULL;
}

int main(void) {
    char input[MAX_INPUT_LEN];

    printf("Enter a string: ");
    if (fgets(input, sizeof(input), stdin) != NULL) {
        size_t len = strcspn(input, "\n");
        input[len] = '\0';

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