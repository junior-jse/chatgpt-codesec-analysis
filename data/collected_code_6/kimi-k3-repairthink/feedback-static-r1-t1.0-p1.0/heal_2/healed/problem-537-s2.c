#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#define MAX_WORD_LEN 100
#define INITIAL_CAPACITY 16

typedef struct {
    char **words;
    size_t count;
    size_t capacity;
} WordList;

static size_t safe_strnlen(const char *s, size_t max_len) {
    size_t len = 0;
    if (s == NULL) {
        return 0;
    }
    while (len < max_len && s[len] != '\0') {
        len++;
    }
    return len;
}

static void safe_strcpy(char *dest, size_t dest_size, const char *src) {
    size_t i = 0;
    if (dest == NULL || src == NULL || dest_size == 0) {
        return;
    }
    while (i + 1 < dest_size && src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

static int is_word_char(char c) {
    return isalnum((unsigned char)c) || c == '_';
}

static void to_lowercase(char *dest, size_t dest_size, const char *src) {
    size_t i;
    size_t len;
    if (dest == NULL || src == NULL || dest_size == 0) {
        return;
    }
    len = safe_strnlen(src, dest_size - 1);
    for (i = 0; i < len; i++) {
        dest[i] = (char)tolower((unsigned char)src[i]);
    }
    dest[i] = '\0';
}

static int find_word(const WordList *list, const char *word) {
    size_t i;
    if (list == NULL || word == NULL) {
        return 0;
    }
    for (i = 0; i < list->count; i++) {
        if (list->words[i] != NULL && strcmp(list->words[i], word) == 0) {
            return 1;
        }
    }
    return 0;
}

static int add_word(WordList *list, const char *word) {
    size_t len;
    size_t new_capacity;
    char **new_words;
    char *copy;

    if (list == NULL || word == NULL) {
        return -1;
    }

    len = safe_strnlen(word, MAX_WORD_LEN);
    if (len == 0 || len >= MAX_WORD_LEN) {
        return -1;
    }

    if (list->count >= list->capacity) {
        new_capacity = (list->capacity == 0) ? INITIAL_CAPACITY : list->capacity * 2;
        if (new_capacity < list->capacity ||
            new_capacity > SIZE_MAX / sizeof(*new_words)) {
            return -1;
        }
        new_words = realloc(list->words, new_capacity * sizeof(*new_words));
        if (new_words == NULL) {
            return -1;
        }
        list->words = new_words;
        list->capacity = new_capacity;
    }

    copy = malloc(len + 1);
    if (copy == NULL) {
        return -1;
    }
    safe_strcpy(copy, len + 1, word);

    list->words[list->count] = copy;
    list->count++;
    return 0;
}

static void free_word_list(WordList *list) {
    size_t i;
    if (list == NULL) {
        return;
    }
    for (i = 0; i < list->count; i++) {
        free(list->words[i]);
    }
    free(list->words);
    list->words = NULL;
    list->count = 0;
    list->capacity = 0;
}

char *find_first_repeated_word(const char *str) {
    WordList word_list;
    char *current_word;
    char *lower_word;
    char *found = NULL;
    size_t i = 0;
    size_t word_len = 0;
    int in_word = 0;

    if (str == NULL) {
        return NULL;
    }

    word_list.words = NULL;
    word_list.count = 0;
    word_list.capacity = 0;

    current_word = malloc(MAX_WORD_LEN);
    if (current_word == NULL) {
        return NULL;
    }

    lower_word = malloc(MAX_WORD_LEN);
    if (lower_word == NULL) {
        free(current_word);
        return NULL;
    }

    while (str[i] != '\0') {
        if (is_word_char(str[i])) {
            if (word_len < MAX_WORD_LEN - 1) {
                current_word[word_len] = str[i];
                word_len++;
            }
            in_word = 1;
        } else {
            if (in_word && word_len > 0) {
                current_word[word_len] = '\0';
                to_lowercase(lower_word, MAX_WORD_LEN, current_word);

                if (find_word(&word_list, lower_word)) {
                    found = malloc(word_len + 1);
                    if (found != NULL) {
                        safe_strcpy(found, word_len + 1, current_word);
                    }
                    goto cleanup;
                }

                if (add_word(&word_list, lower_word) != 0) {
                    goto cleanup;
                }

                word_len = 0;
                in_word = 0;
            }
        }
        i++;
    }

    if (in_word && word_len > 0) {
        current_word[word_len] = '\0';
        to_lowercase(lower_word, MAX_WORD_LEN, current_word);

        if (find_word(&word_list, lower_word)) {
            found = malloc(word_len + 1);
            if (found != NULL) {
                safe_strcpy(found, word_len + 1, current_word);
            }
        }
    }

cleanup:
    free(lower_word);
    free(current_word);
    free_word_list(&word_list);
    return found;
}

int main(void) {
    const char *test_strings[] = {
        "The quick brown fox jumps over the lazy dog",
        "Hello world hello everyone",
        "one two three four five",
        "apple banana apple cherry",
        "This is a test this is only a test"
    };
    size_t num_tests = sizeof(test_strings) / sizeof(test_strings[0]);
    size_t i;

    for (i = 0; i < num_tests; i++) {
        char *repeated;

        printf("String: \"%s\"\n", test_strings[i]);
        repeated = find_first_repeated_word(test_strings[i]);

        if (repeated != NULL && repeated[0] != '\0') {
            printf("First repeated word: \"%s\"\n\n", repeated);
        } else {
            printf("No repeated words found.\n\n");
        }

        free(repeated);
    }

    return 0;
}