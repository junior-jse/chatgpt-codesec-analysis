#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *key;
    char *value;
} KeyValue;

typedef struct {
    KeyValue *entries;
    size_t count;
} Dictionary;

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

static void free_dictionary(Dictionary *dictionary)
{
    size_t i;

    if (dictionary == NULL) {
        return;
    }

    for (i = 0U; i < dictionary->count; ++i) {
        free(dictionary->entries[i].key);
        free(dictionary->entries[i].value);
    }

    free(dictionary->entries);
    dictionary->entries = NULL;
    dictionary->count = 0U;
}

static int tuple_to_dictionary(const char *const tuple[],
                               size_t tuple_size,
                               Dictionary *dictionary)
{
    size_t pair_count;
    size_t i;

    if (dictionary == NULL || (tuple == NULL && tuple_size != 0U)) {
        return -1;
    }

    dictionary->entries = NULL;
    dictionary->count = 0U;

    if ((tuple_size % 2U) != 0U) {
        return -1;
    }

    pair_count = tuple_size / 2U;
    if (pair_count == 0U) {
        return 0;
    }

    if (pair_count > SIZE_MAX / sizeof(*dictionary->entries)) {
        return -1;
    }

    dictionary->entries = calloc(pair_count, sizeof(*dictionary->entries));
    if (dictionary->entries == NULL) {
        return -1;
    }

    dictionary->count = pair_count;

    for (i = 0U; i < pair_count; ++i) {
        if (tuple[2U * i] == NULL || tuple[(2U * i) + 1U] == NULL) {
            free_dictionary(dictionary);
            return -1;
        }

        dictionary->entries[i].key = duplicate_string(tuple[2U * i]);
        if (dictionary->entries[i].key == NULL) {
            free_dictionary(dictionary);
            return -1;
        }

        dictionary->entries[i].value = duplicate_string(tuple[(2U * i) + 1U]);
        if (dictionary->entries[i].value == NULL) {
            free_dictionary(dictionary);
            return -1;
        }
    }

    return 0;
}

int main(void)
{
    const char *tuple[] = {
        "name", "Alice",
        "age", "30",
        "city", "London"
    };
    Dictionary dictionary;
    size_t i;

    if (tuple_to_dictionary(tuple,
                            sizeof(tuple) / sizeof(tuple[0]),
                            &dictionary) != 0) {
        fputs("Failed to convert tuple to dictionary.\n", stderr);
        return EXIT_FAILURE;
    }

    for (i = 0U; i < dictionary.count; ++i) {
        if (printf("%s: %s\n",
                   dictionary.entries[i].key,
                   dictionary.entries[i].value) < 0) {
            free_dictionary(&dictionary);
            return EXIT_FAILURE;
        }
    }

    free_dictionary(&dictionary);
    return EXIT_SUCCESS;
}