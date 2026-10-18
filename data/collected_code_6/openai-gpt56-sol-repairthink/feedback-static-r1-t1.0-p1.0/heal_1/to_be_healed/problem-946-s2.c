#include <ctype.h>
#include <errno.h>
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

static int add_word(WordCount **items, size_t *count, size_t *capacity,
                    const char *word, size_t length)
{
    for (size_t i = 0; i < *count; ++i) {
        /* Possible weaknesses found:
         * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
         */
        if (strlen((*items)[i].word) == length &&
            memcmp((*items)[i].word, word, length) == 0) {
            if ((*items)[i].count == SIZE_MAX) {
                return -1;
            }

            ++(*items)[i].count;
            return 0;
        }
    }

    if (*count == *capacity) {
        size_t new_capacity = *capacity == 0U ? 16U : *capacity * 2U;

        if (new_capacity < *capacity ||
            new_capacity > SIZE_MAX / sizeof(**items)) {
            return -1;
        }

        WordCount *resized = realloc(*items, new_capacity * sizeof(**items));
        if (resized == NULL) {
            return -1;
        }

        *items = resized;
        *capacity = new_capacity;
    }

    if (length == SIZE_MAX) {
        return -1;
    }

    char *copy = malloc(length + 1U);
    if (copy == NULL) {
        return -1;
    }

    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(copy, word, length);
    copy[length] = '\0';

    (*items)[*count].word = copy;
    (*items)[*count].count = 1U;
    ++*count;

    return 0;
}

static int find_most_common(const char *text, WordCount **results,
                            size_t *result_count)
{
    WordCount *items = NULL;
    size_t count = 0U;
    size_t capacity = 0U;
    const char *cursor = text;

    if (text == NULL || results == NULL || result_count == NULL) {
        return -1;
    }

    *results = NULL;
    *result_count = 0U;

    while (*cursor != '\0') {
        while (*cursor != '\0' &&
               !isalnum((unsigned char)*cursor)) {
            ++cursor;
        }

        const char *start = cursor;

        while (*cursor != '\0' &&
               isalnum((unsigned char)*cursor)) {
            ++cursor;
        }

        size_t length = (size_t)(cursor - start);
        if (length == 0U) {
            continue;
        }

        char *normalized = malloc(length + 1U);
        if (normalized == NULL) {
            free_word_counts(items, count);
            return -1;
        }

        for (size_t i = 0; i < length; ++i) {
            normalized[i] = (char)tolower((unsigned char)start[i]);
        }
        normalized[length] = '\0';

        if (add_word(&items, &count, &capacity, normalized, length) != 0) {
            free(normalized);
            free_word_counts(items, count);
            return -1;
        }

        free(normalized);
    }

    if (count == 0U) {
        free(items);
        return 0;
    }

    size_t maximum = 0U;
    for (size_t i = 0; i < count; ++i) {
        if (items[i].count > maximum) {
            maximum = items[i].count;
        }
    }

    size_t matches = 0U;
    for (size_t i = 0; i < count; ++i) {
        if (items[i].count == maximum) {
            ++matches;
        }
    }

    if (matches > SIZE_MAX / sizeof(**results)) {
        free_word_counts(items, count);
        return -1;
    }

    WordCount *common = calloc(matches, sizeof(*common));
    if (common == NULL) {
        free_word_counts(items, count);
        return -1;
    }

    size_t result_index = 0U;
    for (size_t i = 0; i < count; ++i) {
        if (items[i].count == maximum) {
            common[result_index].word = items[i].word;
            common[result_index].count = items[i].count;
            items[i].word = NULL;
            ++result_index;
        }
    }

    free_word_counts(items, count);
    *results = common;
    *result_count = matches;

    return 0;
}

int main(void)
{
    char *text = NULL;
    size_t length = 0U;
    size_t capacity = 0U;
    int character;

    /* Possible weaknesses found:
     * Flawfinder fgetc: Check buffer boundaries if used in a loop including recursive loops (CWE-120, CWE-20). (risk 1, buffer)
     */
    while ((character = fgetc(stdin)) != EOF) {
        if (length == capacity) {
            size_t new_capacity = capacity == 0U ? 256U : capacity * 2U;

            if (new_capacity <= capacity) {
                free(text);
                fputs("Input is too large.\n", stderr);
                return EXIT_FAILURE;
            }

            char *resized = realloc(text, new_capacity);
            if (resized == NULL) {
                free(text);
                fputs("Memory allocation failed.\n", stderr);
                return EXIT_FAILURE;
            }

            text = resized;
            capacity = new_capacity;
        }

        text[length++] = (char)character;
    }

    if (ferror(stdin)) {
        free(text);
        fputs("Failed to read input.\n", stderr);
        return EXIT_FAILURE;
    }

    if (length == SIZE_MAX) {
        free(text);
        fputs("Input is too large.\n", stderr);
        return EXIT_FAILURE;
    }

    char *terminated = realloc(text, length + 1U);
    if (terminated == NULL) {
        free(text);
        fputs("Memory allocation failed.\n", stderr);
        return EXIT_FAILURE;
    }

    text = terminated;
    text[length] = '\0';

    WordCount *results = NULL;
    size_t result_count = 0U;

    if (find_most_common(text, &results, &result_count) != 0) {
        free(text);
        fputs("Failed to process text.\n", stderr);
        return EXIT_FAILURE;
    }

    free(text);

    for (size_t i = 0; i < result_count; ++i) {
        if (printf("%s: %zu\n", results[i].word, results[i].count) < 0) {
            free_word_counts(results, result_count);
            return EXIT_FAILURE;
        }
    }

    free_word_counts(results, result_count);
    return EXIT_SUCCESS;
}