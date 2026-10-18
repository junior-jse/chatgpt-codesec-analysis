#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct {
    char *word;
    size_t count;
} WordEntry;

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

static int find_most_frequent_word(const char *const strings[],
                                   size_t string_count,
                                   char **result,
                                   size_t *occurrences)
{
    WordEntry *entries;
    size_t entry_count = 0U;
    size_t best_index = 0U;
    size_t i;

    if (strings == NULL || result == NULL || occurrences == NULL ||
        string_count == 0U ||
        string_count > SIZE_MAX / sizeof(*entries)) {
        return -1;
    }

    *result = NULL;
    *occurrences = 0U;

    entries = calloc(string_count, sizeof(*entries));
    if (entries == NULL) {
        return -1;
    }

    for (i = 0U; i < string_count; ++i) {
        size_t j;
        char *copy;

        if (strings[i] == NULL) {
            continue;
        }

        for (j = 0U; j < entry_count; ++j) {
            if (strcmp(entries[j].word, strings[i]) == 0) {
                ++entries[j].count;
                break;
            }
        }

        if (j != entry_count) {
            continue;
        }

        copy = duplicate_string(strings[i]);
        if (copy == NULL) {
            size_t k;

            for (k = 0U; k < entry_count; ++k) {
                free(entries[k].word);
            }
            free(entries);
            return -1;
        }

        entries[entry_count].word = copy;
        entries[entry_count].count = 1U;
        ++entry_count;
    }

    if (entry_count == 0U) {
        free(entries);
        return -1;
    }

    for (i = 1U; i < entry_count; ++i) {
        if (entries[i].count > entries[best_index].count) {
            best_index = i;
        }
    }

    *result = duplicate_string(entries[best_index].word);
    if (*result != NULL) {
        *occurrences = entries[best_index].count;
    }

    for (i = 0U; i < entry_count; ++i) {
        free(entries[i].word);
    }
    free(entries);

    return *result == NULL ? -1 : 0;
}

int main(void)
{
    const char *strings[] = {
        "apple", "banana", "apple", "orange",
        "banana", "apple", "pear", "banana"
    };
    const size_t string_count = sizeof(strings) / sizeof(strings[0]);
    char *word = NULL;
    size_t occurrences = 0U;

    if (find_most_frequent_word(strings, string_count, &word, &occurrences) != 0) {
        fputs("Unable to determine the most frequent word.\n", stderr);
        return EXIT_FAILURE;
    }

    if (printf("%s %zu\n", word, occurrences) < 0) {
        free(word);
        return EXIT_FAILURE;
    }

    free(word);
    return EXIT_SUCCESS;
}