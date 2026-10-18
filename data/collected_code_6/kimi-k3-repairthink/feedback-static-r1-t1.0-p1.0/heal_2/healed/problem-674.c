#define _POSIX_C_SOURCE 200809L

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
} WordCollection;

void init_collection(WordCollection *wc, size_t capacity) {
    wc->words = malloc(capacity * sizeof(char *));
    if (wc->words == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    wc->count = 0;
    wc->capacity = capacity;
}

void free_collection(WordCollection *wc) {
    size_t i;
    if (wc == NULL || wc->words == NULL) {
        return;
    }
    for (i = 0; i < wc->count; i++) {
        free(wc->words[i]);
    }
    free(wc->words);
    wc->words = NULL;
    wc->count = 0;
    wc->capacity = 0;
}

int word_exists(const WordCollection *wc, const char *word) {
    size_t i;
    if (wc == NULL || word == NULL) {
        return 0;
    }
    for (i = 0; i < wc->count; i++) {
        if (wc->words[i] != NULL && strcmp(wc->words[i], word) == 0) {
            return 1;
        }
    }
    return 0;
}

int add_word(WordCollection *wc, const char *word) {
    char *new_word;
    char **temp;
    size_t word_len;
    size_t alloc_size;
    int written;

    if (wc == NULL || word == NULL) {
        return 0;
    }

    if (wc->count >= wc->capacity) {
        size_t new_capacity;
        if (wc->capacity > (SIZE_MAX / sizeof(char *)) / 2) {
            return 0;
        }
        new_capacity = (wc->capacity == 0) ? INITIAL_CAPACITY : wc->capacity * 2;
        temp = realloc(wc->words, new_capacity * sizeof(char *));
        if (temp == NULL) {
            return 0;
        }
        wc->words = temp;
        wc->capacity = new_capacity;
    }

    word_len = strnlen(word, MAX_WORD_LEN);
    alloc_size = word_len + 1;
    new_word = malloc(alloc_size);
    if (new_word == NULL) {
        return 0;
    }
    written = snprintf(new_word, alloc_size, "%.*s", (int)word_len, word);
    if (written < 0 || (size_t)written >= alloc_size) {
        free(new_word);
        return 0;
    }
    wc->words[wc->count] = new_word;
    wc->count++;
    return 1;
}

static int append_word(char *output, size_t output_size, size_t *out_idx,
                       const char *word, size_t word_len, int *first_word) {
    size_t space_left;
    size_t prefix;
    size_t needed;

    if (output == NULL || out_idx == NULL || word == NULL || first_word == NULL) {
        return 0;
    }
    if (*out_idx >= output_size) {
        return 0;
    }
    space_left = output_size - *out_idx;
    prefix = (*first_word) ? (size_t)0 : (size_t)1;
    if (word_len > SIZE_MAX - prefix - 1) {
        return 0;
    }
    needed = prefix + word_len + 1;
    if (needed > space_left) {
        return 0;
    }
    if (prefix != 0) {
        output[*out_idx] = ' ';
        (*out_idx)++;
    }
    memcpy(output + *out_idx, word, word_len);
    *out_idx += word_len;
    output[*out_idx] = '\0';
    *first_word = 0;
    return 1;
}

void remove_duplicate_words(const char *input, char *output, size_t output_size) {
    WordCollection seen;
    char buffer[MAX_WORD_LEN];
    size_t buf_idx = 0;
    size_t out_idx = 0;
    size_t i;
    int first_word = 1;

    if (input == NULL || output == NULL || output_size == 0) {
        return;
    }

    output[0] = '\0';
    init_collection(&seen, INITIAL_CAPACITY);

    for (i = 0; input[i] != '\0'; i++) {
        if (isspace((unsigned char)input[i])) {
            if (buf_idx > 0) {
                size_t word_len;
                buffer[buf_idx] = '\0';
                word_len = strnlen(buffer, sizeof(buffer));
                if (!word_exists(&seen, buffer)) {
                    if (!add_word(&seen, buffer)) {
                        free_collection(&seen);
                        return;
                    }
                    append_word(output, output_size, &out_idx, buffer, word_len, &first_word);
                }
                buf_idx = 0;
            }
        } else {
            if (buf_idx + 1 < sizeof(buffer)) {
                buffer[buf_idx++] = input[i];
            }
        }
    }

    if (buf_idx > 0) {
        size_t word_len;
        buffer[buf_idx] = '\0';
        word_len = strnlen(buffer, sizeof(buffer));
        if (!word_exists(&seen, buffer)) {
            append_word(output, output_size, &out_idx, buffer, word_len, &first_word);
        }
    }

    output[output_size - 1] = '\0';
    free_collection(&seen);
}

int main(void) {
    const char *test_string = "hello world hello this is is a test world";
    size_t result_size = strlen(test_string) + 1;
    char *result = malloc(result_size);

    if (result == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    remove_duplicate_words(test_string, result, result_size);
    printf("Original: %s\n", test_string);
    printf("Result:   %s\n", result);

    free(result);
    return 0;
}