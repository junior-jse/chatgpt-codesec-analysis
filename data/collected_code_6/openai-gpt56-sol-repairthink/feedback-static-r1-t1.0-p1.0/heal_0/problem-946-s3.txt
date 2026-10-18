#include <ctype.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *word;
    size_t count;
} WordCount;

static void free_word_counts(WordCount *items, size_t count)
{
    if (items == NULL) {
        return;
    }

    for (size_t i = 0; i < count; ++i) {
        free(items[i].word);
    }

    free(items);
}

static char *copy_word_lowercase(const char *start, size_t length)
{
    if (start == NULL || length == SIZE_MAX) {
        return NULL;
    }

    char *word = malloc(length + 1);
    if (word == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < length; ++i) {
        word[i] = (char)tolower((unsigned char)start[i]);
    }

    word[length] = '\0';
    return word;
}

static int add_word(WordCount **items, size_t *count, size_t *capacity,
                    const char *start, size_t length)
{
    if (items == NULL || count == NULL || capacity == NULL ||
        start == NULL || length == 0) {
        return -1;
    }

    char *word = copy_word_lowercase(start, length);
    if (word == NULL) {
        return -1;
    }

    for (size_t i = 0; i < *count; ++i) {
        if (strcmp((*items)[i].word, word) == 0) {
            if ((*items)[i].count == SIZE_MAX) {
                free(word);
                return -1;
            }

            ++(*items)[i].count;
            free(word);
            return 0;
        }
    }

    if (*count == *capacity) {
        size_t new_capacity = *capacity == 0 ? 16 : *capacity * 2;

        if (new_capacity < *capacity ||
            new_capacity > SIZE_MAX / sizeof(**items)) {
            free(word);
            return -1;
        }

        WordCount *resized = realloc(*items, new_capacity * sizeof(*resized));
        if (resized == NULL) {
            free(word);
            return -1;
        }

        *items = resized;
        *capacity = new_capacity;
    }

    (*items)[*count].word = word;
    (*items)[*count].count = 1;
    ++(*count);
    return 0;
}

static int find_most_common_words(const char *text, WordCount **results,
                                  size_t *result_count)
{
    if (text == NULL || results == NULL || result_count == NULL) {
        return -1;
    }

    *results = NULL;
    *result_count = 0;

    WordCount *items = NULL;
    size_t count = 0;
    size_t capacity = 0;
    const char *cursor = text;

    while (*cursor != '\0') {
        while (*cursor != '\0' && !isalnum((unsigned char)*cursor)) {
            ++cursor;
        }

        const char *start = cursor;

        while (*cursor != '\0' && isalnum((unsigned char)*cursor)) {
            ++cursor;
        }

        size_t length = (size_t)(cursor - start);
        if (length > 0 &&
            add_word(&items, &count, &capacity, start, length) != 0) {
            free_word_counts(items, count);
            return -1;
        }
    }

    if (count == 0) {
        free(items);
        return 0;
    }

    size_t maximum = 0;
    size_t matches = 0;

    for (size_t i = 0; i < count; ++i) {
        if (items[i].count > maximum) {
            maximum = items[i].count;
            matches = 1;
        } else if (items[i].count == maximum) {
            ++matches;
        }
    }

    if (matches > SIZE_MAX / sizeof(**results)) {
        free_word_counts(items, count);
        return -1;
    }

    WordCount *most_common = calloc(matches, sizeof(*most_common));
    if (most_common == NULL) {
        free_word_counts(items, count);
        return -1;
    }

    size_t output_index = 0;

    for (size_t i = 0; i < count; ++i) {
        if (items[i].count == maximum) {
            most_common[output_index] = items[i];
            items[i].word = NULL;
            ++output_index;
        }
    }

    free_word_counts(items, count);
    *results = most_common;
    *result_count = matches;
    return 0;
}

static int read_all_input(FILE *stream, char **text)
{
    if (stream == NULL || text == NULL) {
        return -1;
    }

    *text = NULL;
    size_t length = 0;
    size_t capacity = 4096;
    char *buffer = malloc(capacity);

    if (buffer == NULL) {
        return -1;
    }

    for (;;) {
        if (capacity - length < 2) {
            if (capacity > SIZE_MAX / 2) {
                free(buffer);
                return -1;
            }

            size_t new_capacity = capacity * 2;
            char *resized = realloc(buffer, new_capacity);

            if (resized == NULL) {
                free(buffer);
                return -1;
            }

            buffer = resized;
            capacity = new_capacity;
        }

        size_t amount = fread(buffer + length, 1, capacity - length - 1, stream);
        length += amount;

        if (amount == 0) {
            if (ferror(stream)) {
                free(buffer);
                return -1;
            }
            break;
        }
    }

    buffer[length] = '\0';
    *text = buffer;
    return 0;
}

int main(void)
{
    char *text = NULL;

    if (read_all_input(stdin, &text) != 0) {
        fprintf(stderr, "Failed to read input: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    WordCount *results = NULL;
    size_t result_count = 0;

    if (find_most_common_words(text, &results, &result_count) != 0) {
        fprintf(stderr, "Failed to process input.\n");
        free(text);
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < result_count; ++i) {
        if (printf("%s: %zu\n", results[i].word, results[i].count) < 0) {
            fprintf(stderr, "Failed to write output.\n");
            free_word_counts(results, result_count);
            free(text);
            return EXIT_FAILURE;
        }
    }

    free_word_counts(results, result_count);
    free(text);

    if (fflush(stdout) == EOF) {
        fprintf(stderr, "Failed to flush output.\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}