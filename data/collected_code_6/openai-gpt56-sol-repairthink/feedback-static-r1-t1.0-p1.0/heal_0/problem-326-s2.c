#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct {
    char *word;
    size_t count;
} WordCount;

static char *copy_string(const char *source)
{
    size_t length;
    char *copy;

    if (source == NULL) {
        return NULL;
    }

    length = strlen(source);

    if (length == SIZE_MAX) {
        return NULL;
    }

    copy = malloc(length + 1);
    if (copy == NULL) {
        return NULL;
    }

    memcpy(copy, source, length + 1);
    return copy;
}

static int find_most_frequent_word(const char *const strings[],
                                   size_t string_count,
                                   char **result,
                                   size_t *occurrences)
{
    WordCount *counts = NULL;
    size_t unique_count = 0;
    size_t capacity = 0;
    size_t best_index = 0;
    size_t i;

    if (strings == NULL || string_count == 0 ||
        result == NULL || occurrences == NULL) {
        return -1;
    }

    *result = NULL;
    *occurrences = 0;

    for (i = 0; i < string_count; ++i) {
        size_t j;
        size_t found_index = unique_count;

        if (strings[i] == NULL) {
            goto failure;
        }

        for (j = 0; j < unique_count; ++j) {
            if (strcmp(counts[j].word, strings[i]) == 0) {
                found_index = j;
                break;
            }
        }

        if (found_index < unique_count) {
            if (counts[found_index].count == SIZE_MAX) {
                goto failure;
            }
            ++counts[found_index].count;

            if (counts[found_index].count > counts[best_index].count) {
                best_index = found_index;
            }
        } else {
            char *word_copy;
            WordCount *resized;
            size_t new_capacity;

            if (unique_count == capacity) {
                if (capacity == 0) {
                    new_capacity = 8;
                } else {
                    if (capacity > SIZE_MAX / 2) {
                        goto failure;
                    }
                    new_capacity = capacity * 2;
                }

                if (new_capacity > SIZE_MAX / sizeof(*counts)) {
                    goto failure;
                }

                resized = realloc(counts, new_capacity * sizeof(*counts));
                if (resized == NULL) {
                    goto failure;
                }

                counts = resized;
                capacity = new_capacity;
            }

            word_copy = copy_string(strings[i]);
            if (word_copy == NULL) {
                goto failure;
            }

            counts[unique_count].word = word_copy;
            counts[unique_count].count = 1;

            if (unique_count == 0) {
                best_index = 0;
            }

            ++unique_count;
        }
    }

    *result = copy_string(counts[best_index].word);
    if (*result == NULL) {
        goto failure;
    }

    *occurrences = counts[best_index].count;

    for (i = 0; i < unique_count; ++i) {
        free(counts[i].word);
    }
    free(counts);

    return 0;

failure:
    for (i = 0; i < unique_count; ++i) {
        free(counts[i].word);
    }
    free(counts);
    free(*result);
    *result = NULL;
    *occurrences = 0;

    return -1;
}

int main(void)
{
    const char *strings[] = {
        "apple", "banana", "apple", "orange",
        "banana", "apple", "grape", "banana", "apple"
    };
    const size_t string_count = sizeof(strings) / sizeof(strings[0]);
    char *word = NULL;
    size_t occurrences = 0;

    if (find_most_frequent_word(strings, string_count,
                                &word, &occurrences) != 0) {
        fputs("Unable to determine the most frequent word.\n", stderr);
        return EXIT_FAILURE;
    }

    if (printf("%s: %zu\n", word, occurrences) < 0) {
        free(word);
        return EXIT_FAILURE;
    }

    free(word);
    return EXIT_SUCCESS;
}