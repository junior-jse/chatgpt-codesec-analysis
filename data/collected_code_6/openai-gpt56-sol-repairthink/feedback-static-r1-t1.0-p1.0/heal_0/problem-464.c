#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *key;
    char *value;
} DictionaryEntry;

typedef struct {
    DictionaryEntry *entries;
    size_t size;
} Dictionary;

static char *duplicate_string(const char *source)
{
    size_t length;
    char *copy;

    if (source == NULL) {
        return NULL;
    }

    length = strlen(source);
    copy = malloc(length + 1U);
    if (copy == NULL) {
        return NULL;
    }

    memcpy(copy, source, length + 1U);
    return copy;
}

static void dictionary_destroy(Dictionary *dictionary)
{
    size_t index;

    if (dictionary == NULL) {
        return;
    }

    for (index = 0U; index < dictionary->size; ++index) {
        free(dictionary->entries[index].key);
        free(dictionary->entries[index].value);
    }

    free(dictionary->entries);
    dictionary->entries = NULL;
    dictionary->size = 0U;
}

static bool dictionary_create(
    Dictionary *dictionary,
    const char *const keys[],
    const char *const values[],
    size_t size)
{
    size_t index;

    if (dictionary == NULL || (size > 0U && (keys == NULL || values == NULL))) {
        return false;
    }

    dictionary->entries = NULL;
    dictionary->size = 0U;

    if (size == 0U) {
        return true;
    }

    if (size > SIZE_MAX / sizeof(*dictionary->entries)) {
        return false;
    }

    dictionary->entries = calloc(size, sizeof(*dictionary->entries));
    if (dictionary->entries == NULL) {
        return false;
    }

    dictionary->size = size;

    for (index = 0U; index < size; ++index) {
        if (keys[index] == NULL || values[index] == NULL) {
            dictionary_destroy(dictionary);
            return false;
        }

        dictionary->entries[index].key = duplicate_string(keys[index]);
        if (dictionary->entries[index].key == NULL) {
            dictionary_destroy(dictionary);
            return false;
        }

        dictionary->entries[index].value = duplicate_string(values[index]);
        if (dictionary->entries[index].value == NULL) {
            dictionary_destroy(dictionary);
            return false;
        }
    }

    return true;
}

static bool dictionary_all_values_same(const Dictionary *dictionary)
{
    size_t index;
    const char *first_value;

    if (dictionary == NULL || (dictionary->size > 0U && dictionary->entries == NULL)) {
        return false;
    }

    if (dictionary->size <= 1U) {
        return true;
    }

    first_value = dictionary->entries[0].value;
    if (first_value == NULL) {
        return false;
    }

    for (index = 1U; index < dictionary->size; ++index) {
        if (dictionary->entries[index].value == NULL ||
            strcmp(first_value, dictionary->entries[index].value) != 0) {
            return false;
        }
    }

    return true;
}

int main(void)
{
    const char *keys[] = {"first", "second", "third"};
    const char *values[] = {"same", "same", "same"};
    Dictionary dictionary;
    int result;

    if (!dictionary_create(&dictionary, keys, values, 3U)) {
        fputs("Failed to create dictionary.\n", stderr);
        return EXIT_FAILURE;
    }

    result = printf(
        "%s\n",
        dictionary_all_values_same(&dictionary) ? "All values are the same."
                                                : "Values are not all the same.");

    dictionary_destroy(&dictionary);

    if (result < 0) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}