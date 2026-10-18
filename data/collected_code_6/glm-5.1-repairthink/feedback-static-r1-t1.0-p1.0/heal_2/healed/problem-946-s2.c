#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    char *word;
    int count;
} WordCount;

char *str_dup(const char *str) {
    size_t max_len = 255;
    size_t len = strnlen(str, max_len);
    char *copy = malloc(len + 1);
    if (copy) {
        snprintf(copy, len + 1, "%s", str);
    }
    return copy;
}

int add_word(WordCount **words, size_t *unique_count, size_t *capacity, const char *buffer) {
    for (size_t j = 0; j < *unique_count; j++) {
        if (strcmp((*words)[j].word, buffer) == 0) {
            (*words)[j].count++;
            return 0;
        }
    }

    if (*unique_count >= *capacity) {
        size_t new_capacity = *capacity == 0 ? 16 : *capacity * 2;
        WordCount *new_words = realloc(*words, new_capacity * sizeof(WordCount));
        if (new_words == NULL) {
            return -1;
        }
        *words = new_words;
        *capacity = new_capacity;
    }

    (*words)[*unique_count].word = str_dup(buffer);
    if ((*words)[*unique_count].word == NULL) {
        return -1;
    }
    (*words)[*unique_count].count = 1;
    (*unique_count)++;
    return 0;
}

void free_words(WordCount *words, size_t unique_count) {
    for (size_t i = 0; i < unique_count; i++) {
        free(words[i].word);
    }
    free(words);
}

void find_most_common(const char *text) {
    if (text == NULL) {
        return;
    }

    WordCount *words = NULL;
    size_t unique_count = 0;
    size_t capacity = 0;
    size_t buf_cap = 256;
    char *buffer = malloc(buf_cap);
    if (buffer == NULL) {
        return;
    }
    size_t buf_idx = 0;

    for (size_t i = 0; text[i] != '\0'; i++) {
        if (isalpha((unsigned char)text[i])) {
            if (buf_idx < buf_cap - 1) {
                buffer[buf_idx++] = tolower((unsigned char)text[i]);
            }
        } else {
            if (buf_idx > 0) {
                buffer[buf_idx] = '\0';
                buf_idx = 0;
                if (add_word(&words, &unique_count, &capacity, buffer) != 0) {
                    free(buffer);
                    free_words(words, unique_count);
                    return;
                }
            }
        }
    }

    if (buf_idx > 0) {
        buffer[buf_idx] = '\0';
        if (add_word(&words, &unique_count, &capacity, buffer) != 0) {
            free(buffer);
            free_words(words, unique_count);
            return;
        }
    }

    int max_count = 0;
    for (size_t i = 0; i < unique_count; i++) {
        if (words[i].count > max_count) {
            max_count = words[i].count;
        }
    }

    if (max_count > 0) {
        printf("Most common elements (count: %d):\n", max_count);
        for (size_t i = 0; i < unique_count; i++) {
            if (words[i].count == max_count) {
                printf("%s\n", words[i].word);
            }
        }
    }

    free(buffer);
    free_words(words, unique_count);
}

int main(void) {
    const char *sample_text = "Hello world! This is a test. Hello again, this test is only a test.";
    find_most_common(sample_text);
    return 0;
}