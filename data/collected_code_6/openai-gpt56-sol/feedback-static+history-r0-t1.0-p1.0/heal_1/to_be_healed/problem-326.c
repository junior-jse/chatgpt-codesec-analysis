#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *word;
    size_t count;
    size_t order;
} WordCount;

static char *duplicate_string(const char *source)
{
    size_t length;
    char *copy;

    if (source == NULL) {
        return NULL;
    }

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    length = strlen(source);
    if (length == SIZE_MAX) {
        return NULL;
    }

    copy = malloc(length + 1U);
    if (copy == NULL) {
        return NULL;
    }

    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(copy, source, length + 1U);
    return copy;
}

static int ensure_capacity(WordCount **items, size_t *capacity, size_t required)
{
    WordCount *resized;
    size_t new_capacity;

    if (items == NULL || capacity == NULL) {
        return 0;
    }

    if (required <= *capacity) {
        return 1;
    }

    new_capacity = (*capacity == 0U) ? 8U : *capacity;
    while (new_capacity < required) {
        if (new_capacity > SIZE_MAX / 2U) {
            new_capacity = required;
            break;
        }
        new_capacity *= 2U;
    }

    if (new_capacity > SIZE_MAX / sizeof(**items)) {
        return 0;
    }

    resized = realloc(*items, new_capacity * sizeof(**items));
    if (resized == NULL) {
        return 0;
    }

    *items = resized;
    *capacity = new_capacity;
    return 1;
}

static void free_word_counts(WordCount *items, size_t count)
{
    size_t index;

    if (items == NULL) {
        return;
    }

    for (index = 0U; index < count; ++index) {
        free(items[index].word);
    }

    free(items);
}

char *most_frequent_word(const char *const strings[], size_t string_count)
{
    WordCount *items = NULL;
    size_t item_count = 0U;
    size_t capacity = 0U;
    size_t order = 0U;
    size_t string_index;
    size_t best_index;

    if (strings == NULL || string_count == 0U) {
        return NULL;
    }

    for (string_index = 0U; string_index < string_count; ++string_index) {
        size_t item_index;

        if (strings[string_index] == NULL) {
            free_word_counts(items, item_count);
            return NULL;
        }

        for (item_index = 0U; item_index < item_count; ++item_index) {
            if (strcmp(items[item_index].word, strings[string_index]) == 0) {
                if (items[item_index].count == SIZE_MAX) {
                    free_word_counts(items, item_count);
                    return NULL;
                }
                ++items[item_index].count;
                break;
            }
        }

        if (item_index == item_count) {
            char *word_copy;

            if (!ensure_capacity(&items, &capacity, item_count + 1U)) {
                free_word_counts(items, item_count);
                return NULL;
            }

            word_copy = duplicate_string(strings[string_index]);
            if (word_copy == NULL) {
                free_word_counts(items, item_count);
                return NULL;
            }

            items[item_count].word = word_copy;
            items[item_count].count = 1U;
            items[item_count].order = order;
            ++item_count;
            ++order;
        }
    }

    if (item_count == 0U) {
        free(items);
        return NULL;
    }

    best_index = 0U;
    for (string_index = 1U; string_index < item_count; ++string_index) {
        if (items[string_index].count > items[best_index].count ||
            (items[string_index].count == items[best_index].count &&
             items[string_index].order < items[best_index].order)) {
            best_index = string_index;
        }
    }

    {
        char *result = duplicate_string(items[best_index].word);
        free_word_counts(items, item_count);
        return result;
    }
}

int main(void)
{
    const char *strings[] = {
        "apple",
        "banana",
        "apple",
        "orange",
        "banana",
        "apple"
    };
    const size_t string_count = sizeof(strings) / sizeof(strings[0]);
    char *word = most_frequent_word(strings, string_count);

    if (word == NULL) {
        fputs("Unable to determine the most frequent word.\n", stderr);
        return EXIT_FAILURE;
    }

    if (printf("%s\n", word) < 0) {
        free(word);
        return EXIT_FAILURE;
    }

    free(word);
    return EXIT_SUCCESS;
}