#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 100
#define MAX_WORDS 1000

char* find_first_repeated_word(const char* str) {
    if (str == NULL) {
        return NULL;
    }

    char words[MAX_WORDS][MAX_WORD_LEN];
    int word_count = 0;
    char current_word[MAX_WORD_LEN];
    int current_len = 0;
    int i = 0;

    while (str[i] != '\0') {
        if (isalpha(str[i])) {
            if (current_len < MAX_WORD_LEN - 1) {
                current_word[current_len++] = tolower(str[i]);
            }
        } else {
            if (current_len > 0) {
                current_word[current_len] = '\0';

                for (int j = 0; j < word_count; j++) {
                    if (strcmp(words[j], current_word) == 0) {
                        char* result = malloc(strlen(current_word) + 1);
                        if (result != NULL) {
                            strcpy(result, current_word);
                        }
                        return result;
                    }
                }

                if (word_count < MAX_WORDS) {
                    strcpy(words[word_count], current_word);
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
            if (strcmp(words[j], current_word) == 0) {
                char* result = malloc(strlen(current_word) + 1);
                if (result != NULL) {
                    strcpy(result, current_word);
                }
                return result;
            }
        }
    }

    return NULL;
}

int main(void) {
    char input[1000];

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