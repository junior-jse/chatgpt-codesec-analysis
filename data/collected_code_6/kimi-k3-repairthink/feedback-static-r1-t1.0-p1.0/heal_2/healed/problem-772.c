#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 256
#define MAX_INPUT_LEN 65536

static size_t bounded_strlen(const char *s, size_t maxlen) {
    size_t n = 0;

    if (s == NULL) {
        return 0;
    }

    while (n < maxlen && s[n] != '\0') {
        n++;
    }

    return n;
}

char *remove_words_by_length(const char *str, size_t k) {
    if (str == NULL) {
        return NULL;
    }

    size_t len = bounded_strlen(str, MAX_INPUT_LEN);
    if (len == MAX_INPUT_LEN) {
        return NULL;
    }

    if (len == 0) {
        char *empty = malloc(1);
        if (empty != NULL) {
            empty[0] = '\0';
        }
        return empty;
    }

    size_t capacity = len + 1;
    char *result = malloc(capacity);
    if (result == NULL) {
        return NULL;
    }

    size_t i = 0;
    size_t out_idx = 0;
    int first_word = 1;

    while (i < len) {
        while (i < len && isspace((unsigned char)str[i])) {
            i++;
        }

        if (i >= len) {
            break;
        }

        size_t word_start = i;
        size_t word_len = 0;

        while (i < len && !isspace((unsigned char)str[i])) {
            word_len++;
            i++;
        }

        if (word_len != k) {
            size_t separator = first_word ? 0u : 1u;
            size_t remaining = capacity - out_idx;

            if (separator + word_len + 1 > remaining) {
                break;
            }

            if (!first_word) {
                result[out_idx++] = ' ';
            }

            memcpy(result + out_idx, str + word_start, word_len);
            out_idx += word_len;
            first_word = 0;
        }
    }

    result[out_idx] = '\0';

    char *final_result = realloc(result, out_idx + 1);
    if (final_result == NULL) {
        return result;
    }

    return final_result;
}

int main(void) {
    const char *test_str = "This is a sample string with some words of different lengths";
    size_t k = 4;

    char *result = remove_words_by_length(test_str, k);

    if (result != NULL) {
        printf("Original: %s\n", test_str);
        printf("Modified: %s\n", result);
        free(result);
    } else {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}