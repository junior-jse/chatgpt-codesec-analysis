#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1000

static size_t bounded_strlen(const char* s, size_t max_len) {
    size_t len = 0;
    while (len < max_len && s[len] != '\0') {
        len++;
    }
    return len;
}

static char* duplicate_word(const char* word, size_t max_len) {
    size_t len = bounded_strlen(word, max_len);
    char* copy = malloc(len + 1);
    if (copy != NULL) {
        memcpy(copy, word, len);
        copy[len] = '\0';
    }
    return copy;
}

char* find_first_repeated_word(const char* str) {
    if (str == NULL) {
        return NULL;
    }

    size_t str_len = bounded_strlen(str, MAX_INPUT_LEN);

    size_t words_capacity = 16;
    size_t word_count = 0;
    char** words = malloc(words_capacity * sizeof(*words));
    if (words == NULL) {
        return NULL;
    }

    size_t word_capacity = 16;
    size_t current_len = 0;
    char* current_word = malloc(word_capacity);
    if (current_word == NULL) {
        free(words);
        return NULL;
    }

    char* result = NULL;

    for (size_t i = 0; i <= str_len; i++) {
        unsigned char ch = (i < str_len) ? (unsigned char)str[i] : (unsigned char)'\0';

        if (ch != '\0' && isalpha(ch)) {
            if (current_len + 1 >= word_capacity) {
                size_t new_capacity = word_capacity * 2;
                char* resized = realloc(current_word, new_capacity);
                if (resized == NULL) {
                    goto cleanup;
                }
                current_word = resized;
                word_capacity = new_capacity;
            }
            current_word[current_len++] = (char)tolower(ch);
        } else {
            if (current_len > 0) {
                current_word[current_len] = '\0';

                for (size_t j = 0; j < word_count; j++) {
                    if (strcmp(words[j], current_word) == 0) {
                        result = duplicate_word(current_word, current_len + 1);
                        goto cleanup;
                    }
                }

                if (word_count >= words_capacity) {
                    size_t new_capacity = words_capacity * 2;
                    char** resized = realloc(words, new_capacity * sizeof(*words));
                    if (resized == NULL) {
                        goto cleanup;
                    }
                    words = resized;
                    words_capacity = new_capacity;
                }

                words[word_count] = duplicate_word(current_word, current_len + 1);
                if (words[word_count] == NULL) {
                    goto cleanup;
                }
                word_count++;

                current_len = 0;
            }

            if (ch == '\0') {
                break;
            }
        }
    }

cleanup:
    for (size_t j = 0; j < word_count; j++) {
        free(words[j]);
    }
    free(words);
    free(current_word);
    return result;
}

int main(void) {
    char* input = malloc(MAX_INPUT_LEN);
    if (input == NULL) {
        fprintf(stderr, "Error: out of memory.\n");
        return EXIT_FAILURE;
    }

    printf("Enter a string: ");
    if (fgets(input, MAX_INPUT_LEN, stdin) != NULL) {
        input[strcspn(input, "\n")] = '\0';

        char* result = find_first_repeated_word(input);

        if (result != NULL) {
            printf("First repeated word: %s\n", result);
            free(result);
        } else {
            printf("No repeated word found.\n");
        }
    }

    free(input);
    return EXIT_SUCCESS;
}