#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *key;
    char *value;
} DictionaryEntry;

typedef struct {
    DictionaryEntry *entries;
    size_t count;
} Dictionary;

static char *duplicate_string(const char *source)
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

    for (index = 0U; index < dictionary->count; ++index) {
        free(dictionary->entries[index].key);
        free(dictionary->entries[index].value);
    }

    free(dictionary->entries);
    dictionary->entries = NULL;
    dictionary->count = 0U;
}

static int dictionary_initialize(
    Dictionary *dictionary,
    const char *const keys[],
    const char *const values[],
    size_t count)
{
    size_t index;

    if (dictionary == NULL || (count > 0U && (keys == NULL || values == NULL))) {
        return -1;
    }

    dictionary->entries = NULL;
    dictionary->count = 0U;

    if (count == 0U) {
        return 0;
    }

    if (count > SIZE_MAX / sizeof(*dictionary->entries)) {
        return -1;
    }

    dictionary->entries = calloc(count, sizeof(*dictionary->entries));
    if (dictionary->entries == NULL) {
        return -1;
    }

    for (index = 0U; index < count; ++index) {
        if (keys[index] == NULL || values[index] == NULL) {
            dictionary->count = index;
            dictionary_destroy(dictionary);
            return -1;
        }

        dictionary->entries[index].key = duplicate_string(keys[index]);
        if (dictionary->entries[index].key == NULL) {
            dictionary->count = index + 1U;
            dictionary_destroy(dictionary);
            return -1;
        }

        dictionary->entries[index].value = duplicate_string(values[index]);
        if (dictionary->entries[index].value == NULL) {
            dictionary->count = index + 1U;
            dictionary_destroy(dictionary);
            return -1;
        }
    }

    dictionary->count = count;
    return 0;
}

static const DictionaryEntry *dictionary_entry_at(
    const Dictionary *dictionary,
    size_t index)
{
    if (dictionary == NULL || index >= dictionary->count) {
        return NULL;
    }

    return &dictionary->entries[index];
}

int main(void)
{
    const char *keys[] = {"name", "language", "version"};
    const char *values[] = {"Example", "C", "17"};
    const size_t item_count = sizeof(keys) / sizeof(keys[0]);
    Dictionary dictionary = {NULL, 0U};
    const DictionaryEntry *entry;
    size_t index = 1U;

    if (dictionary_initialize(&dictionary, keys, values, item_count) != 0) {
        fputs("Failed to initialize dictionary.\n", stderr);
        return EXIT_FAILURE;
    }

    entry = dictionary_entry_at(&dictionary, index);
    if (entry == NULL) {
        fputs("Dictionary index is out of range.\n", stderr);
        dictionary_destroy(&dictionary);
        return EXIT_FAILURE;
    }

    if (printf("%s: %s\n", entry->key, entry->value) < 0) {
        fputs("Failed to write output.\n", stderr);
        dictionary_destroy(&dictionary);
        return EXIT_FAILURE;
    }

    dictionary_destroy(&dictionary);
    return EXIT_SUCCESS;
}