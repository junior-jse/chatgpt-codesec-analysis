#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#define DELIMITERS " \t\n\r.,;:!?\"'()[]{}"

static char *read_line(void) {
    size_t size = 128;
    size_t pos = 0;
    char *buffer = malloc(size);
    if (buffer == NULL) return NULL;

    int c;
    while ((c = getchar()) != EOF) {
        if (pos + 1 >= size) {
            if (size > SIZE_MAX / 2) {
                free(buffer);
                return NULL;
            }
            size_t new_size = size * 2;
            char *temp = realloc(buffer, new_size);
            if (temp == NULL) {
                free(buffer);
                return NULL;
            }
            buffer = temp;
            size = new_size;
        }
        buffer[pos++] = (char)c;
        if (c == '\n') {
            buffer[pos - 1] = '\0';
            return buffer;
        }
    }

    if (pos == 0) {
        free(buffer);
        return NULL;
    }

    buffer[pos] = '\0';
    return buffer;
}

char *find_first_repeated_word(const char *str) {
    if (str == NULL || *str == '\0') return NULL;

    char *str_copy = strdup(str);
    if (str_copy == NULL) return NULL;

    char **words = NULL;
    size_t words_cap = 0;
    size_t word_cnt = 0;
    char *token = strtok(str_copy, DELIMITERS);

    while (token != NULL) {
        char *lower_token = strdup(token);
        if (lower_token == NULL) goto cleanup_error;

        for (char *p = lower_token; *p; p++) {
            *p = (char)tolower((unsigned char)*p);
        }

        for (size_t i = 0; i < word_cnt; i++) {
            if (strcmp(words[i], lower_token) == 0) {
                char *result = strdup(token);
                free(lower_token);
                for (size_t j = 0; j < word_cnt; j++) free(words[j]);
                free(words);
                free(str_copy);
                if (result == NULL) return NULL;
                return result;
            }
        }

        if (word_cnt >= words_cap) {
            size_t new_cap = (words_cap == 0) ? 16 : words_cap * 2;
            if (new_cap > SIZE_MAX / sizeof(char *)) {
                free(lower_token);
                goto cleanup_error;
            }
            char **temp = realloc(words, new_cap * sizeof(char *));
            if (temp == NULL) {
                free(lower_token);
                goto cleanup_error;
            }
            words = temp;
            words_cap = new_cap;
        }
        words[word_cnt++] = lower_token;
        token = strtok(NULL, DELIMITERS);
    }

    free(str_copy);
    for (size_t i = 0; i < word_cnt; i++) free(words[i]);
    free(words);
    return NULL;

cleanup_error:
    free(str_copy);
    for (size_t i = 0; i < word_cnt; i++) free(words[i]);
    free(words);
    return NULL;
}

int main(void) {
    printf("Enter a string: ");
    char *input = read_line();
    if (input == NULL) {
        printf("Error reading input.\n");
        return 1;
    }

    char *result = find_first_repeated_word(input);
    if (result != NULL) {
        printf("First repeated word: %s\n", result);
        free(result);
    } else {
        printf("No repeated word found.\n");
    }

    free(input);
    return 0;
}