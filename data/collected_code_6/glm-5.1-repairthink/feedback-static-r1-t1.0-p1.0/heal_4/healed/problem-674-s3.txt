#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
    char *word;
    size_t length;
} WordEntry;

char *remove_duplicate_words(const char *input) {
    if (input == NULL) {
        return NULL;
    }

    size_t len = strnlen(input, SIZE_MAX);
    char *temp = malloc(len + 1);
    if (temp == NULL) {
        return NULL;
    }

    memcpy(temp, input, len + 1);

    size_t max_words = len / 2 + 1;
    WordEntry *words = malloc(max_words * sizeof(WordEntry));
    if (words == NULL) {
        free(temp);
        return NULL;
    }

    size_t word_count = 0;
    char *token = strtok(temp, " \t\n\r");
    while (token != NULL) {
        size_t token_len = strlen(token);
        bool found = false;
        for (size_t i = 0; i < word_count; i++) {
            if (words[i].length == token_len && memcmp(words[i].word, token, token_len) == 0) {
                found = true;
                break;
            }
        }

        if (!found) {
            words[word_count].word = token;
            words[word_count].length = token_len;
            word_count++;
        }

        token = strtok(NULL, " \t\n\r");
    }

    size_t result_size = 0;
    for (size_t i = 0; i < word_count; i++) {
        result_size += words[i].length;
    }
    if (word_count > 0) {
        result_size += word_count - 1;
    }

    char *result = malloc(result_size + 1);
    if (result == NULL) {
        free(temp);
        free(words);
        return NULL;
    }

    size_t offset = 0;
    for (size_t i = 0; i < word_count; i++) {
        if (offset + words[i].length <= result_size) {
            memcpy(result + offset, words[i].word, words[i].length);
        }
        offset += words[i].length;
        if (i < word_count - 1) {
            if (offset < result_size) {
                result[offset] = ' ';
            }
            offset++;
        }
    }
    result[result_size] = '\0';

    free(temp);
    free(words);

    return result;
}

int main(void) {
    char str[] = "hello world hello there world again";
    char *cleaned = remove_duplicate_words(str);

    if (cleaned != NULL) {
        printf("Original: %s\n", str);
        printf("Cleaned: %s\n", cleaned);
        free(cleaned);
    }

    const char *null_test = remove_duplicate_words(NULL);
    if (null_test == NULL) {
        printf("NULL input handled safely.\n");
    }

    return 0;
}