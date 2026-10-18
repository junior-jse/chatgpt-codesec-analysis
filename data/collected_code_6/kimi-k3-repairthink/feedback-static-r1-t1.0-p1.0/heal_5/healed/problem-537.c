#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#define INITIAL_WORDS_CAPACITY 16
#define INITIAL_WORD_CAPACITY 32

static size_t bounded_strlen(const char *s, size_t max_len) {
    size_t len = 0;

    while (len < max_len && s[len] != '\0') {
        len++;
    }
    return len;
}

static char *safe_copy_word(const char *src, size_t dest_capacity) {
    size_t src_len;
    size_t k;
    char *dest;

    if (src == NULL || dest_capacity == 0) {
        return NULL;
    }

    src_len = bounded_strlen(src, dest_capacity);
    if (src_len >= dest_capacity) {
        return NULL;
    }

    dest = malloc(dest_capacity);
    if (dest == NULL) {
        return NULL;
    }

    for (k = 0; k < src_len; k++) {
        dest[k] = src[k];
    }
    dest[src_len] = '\0';

    return dest;
}

char *find_first_repeated_word(const char *str) {
    char **words = NULL;
    size_t word_count = 0;
    size_t words_capacity = 0;
    char *current_word = NULL;
    size_t word_len = 0;
    size_t word_capacity = 0;
    size_t i;
    size_t j;
    char *result = NULL;

    if (str == NULL) {
        return NULL;
    }

    words_capacity = INITIAL_WORDS_CAPACITY;
    words = malloc(words_capacity * sizeof(*words));
    if (words == NULL) {
        return NULL;
    }

    word_capacity = INITIAL_WORD_CAPACITY;
    current_word = malloc(word_capacity);
    if (current_word == NULL) {
        free(words);
        return NULL;
    }

    for (i = 0; str[i] != '\0'; i++) {
        if (isalpha((unsigned char)str[i])) {
            if (word_len + 1 >= word_capacity) {
                size_t new_capacity;
                char *new_word;

                if (word_capacity > SIZE_MAX / 2) {
                    goto cleanup;
                }
                new_capacity = word_capacity * 2;
                new_word = realloc(current_word, new_capacity);
                if (new_word == NULL) {
                    goto cleanup;
                }
                current_word = new_word;
                word_capacity = new_capacity;
            }
            current_word[word_len++] = (char)tolower((unsigned char)str[i]);
        } else {
            if (word_len > 0) {
                current_word[word_len] = '\0';

                for (j = 0; j < word_count; j++) {
                    if (strcmp(words[j], current_word) == 0) {
                        result = safe_copy_word(current_word, word_len + 1);
                        goto cleanup;
                    }
                }

                if (word_count >= words_capacity) {
                    size_t new_capacity;
                    char **new_words;

                    if (words_capacity > SIZE_MAX / (2 * sizeof(*words))) {
                        goto cleanup;
                    }
                    new_capacity = words_capacity * 2;
                    new_words = realloc(words, new_capacity * sizeof(*new_words));
                    if (new_words == NULL) {
                        goto cleanup;
                    }
                    words = new_words;
                    words_capacity = new_capacity;
                }

                words[word_count] = safe_copy_word(current_word, word_len + 1);
                if (words[word_count] == NULL) {
                    goto cleanup;
                }
                word_count++;

                word_len = 0;
            }
        }
    }

    if (word_len > 0) {
        current_word[word_len] = '\0';

        for (j = 0; j < word_count; j++) {
            if (strcmp(words[j], current_word) == 0) {
                result = safe_copy_word(current_word, word_len + 1);
                goto cleanup;
            }
        }
    }

cleanup:
    for (j = 0; j < word_count; j++) {
        free(words[j]);
    }
    free(words);
    free(current_word);
    return result;
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