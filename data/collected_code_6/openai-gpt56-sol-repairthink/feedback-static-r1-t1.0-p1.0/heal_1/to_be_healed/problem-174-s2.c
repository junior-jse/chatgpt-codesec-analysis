#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    const char *key;
    const char *value;
} KeyValuePair;

typedef struct {
    char *key;
    char **values;
    size_t value_count;
    size_t value_capacity;
} DictionaryEntry;

typedef struct {
    DictionaryEntry *entries;
    size_t entry_count;
    size_t entry_capacity;
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

static void dictionary_destroy(Dictionary *dictionary)
{
    size_t i;
    size_t j;

    if (dictionary == NULL) {
        return;
    }

    for (i = 0U; i < dictionary->entry_count; ++i) {
        free(dictionary->entries[i].key);
        for (j = 0U; j < dictionary->entries[i].value_count; ++j) {
            free(dictionary->entries[i].values[j]);
        }
        free(dictionary->entries[i].values);
    }

    free(dictionary->entries);
    dictionary->entries = NULL;
    dictionary->entry_count = 0U;
    dictionary->entry_capacity = 0U;
}

static DictionaryEntry *dictionary_find_entry(Dictionary *dictionary,
                                              const char *key)
{
    size_t i;

    if (dictionary == NULL || key == NULL) {
        return NULL;
    }

    for (i = 0U; i < dictionary->entry_count; ++i) {
        if (strcmp(dictionary->entries[i].key, key) == 0) {
            return &dictionary->entries[i];
        }
    }

    return NULL;
}

static int dictionary_grow_entries(Dictionary *dictionary)
{
    size_t new_capacity;
    DictionaryEntry *new_entries;

    if (dictionary == NULL) {
        return -1;
    }

    if (dictionary->entry_capacity == 0U) {
        new_capacity = 4U;
    } else {
        if (dictionary->entry_capacity > SIZE_MAX / 2U) {
            return -1;
        }
        new_capacity = dictionary->entry_capacity * 2U;
    }

    if (new_capacity > SIZE_MAX / sizeof(*new_entries)) {
        return -1;
    }

    new_entries = realloc(dictionary->entries,
                          new_capacity * sizeof(*new_entries));
    if (new_entries == NULL) {
        return -1;
    }

    dictionary->entries = new_entries;
    dictionary->entry_capacity = new_capacity;
    return 0;
}

static int entry_grow_values(DictionaryEntry *entry)
{
    size_t new_capacity;
    char **new_values;

    if (entry == NULL) {
        return -1;
    }

    if (entry->value_capacity == 0U) {
        new_capacity = 4U;
    } else {
        if (entry->value_capacity > SIZE_MAX / 2U) {
            return -1;
        }
        new_capacity = entry->value_capacity * 2U;
    }

    if (new_capacity > SIZE_MAX / sizeof(*new_values)) {
        return -1;
    }

    new_values = realloc(entry->values,
                         new_capacity * sizeof(*new_values));
    if (new_values == NULL) {
        return -1;
    }

    entry->values = new_values;
    entry->value_capacity = new_capacity;
    return 0;
}

static int entry_add_value(DictionaryEntry *entry, const char *value)
{
    char *value_copy;

    if (entry == NULL || value == NULL) {
        return -1;
    }

    if (entry->value_count == entry->value_capacity &&
        entry_grow_values(entry) != 0) {
        return -1;
    }

    value_copy = duplicate_string(value);
    if (value_copy == NULL) {
        return -1;
    }

    entry->values[entry->value_count] = value_copy;
    ++entry->value_count;
    return 0;
}

static int dictionary_add(Dictionary *dictionary,
                          const char *key,
                          const char *value)
{
    DictionaryEntry *entry;
    char *key_copy;

    if (dictionary == NULL || key == NULL || value == NULL) {
        return -1;
    }

    entry = dictionary_find_entry(dictionary, key);
    if (entry != NULL) {
        return entry_add_value(entry, value);
    }

    if (dictionary->entry_count == dictionary->entry_capacity &&
        dictionary_grow_entries(dictionary) != 0) {
        return -1;
    }

    key_copy = duplicate_string(key);
    if (key_copy == NULL) {
        return -1;
    }

    entry = &dictionary->entries[dictionary->entry_count];
    entry->key = key_copy;
    entry->values = NULL;
    entry->value_count = 0U;
    entry->value_capacity = 0U;

    if (entry_add_value(entry, value) != 0) {
        free(entry->key);
        entry->key = NULL;
        return -1;
    }

    ++dictionary->entry_count;
    return 0;
}

static int group_pairs(const KeyValuePair *pairs,
                       size_t pair_count,
                       Dictionary *result)
{
    size_t i;

    if (result == NULL || (pairs == NULL && pair_count != 0U)) {
        return -1;
    }

    result->entries = NULL;
    result->entry_count = 0U;
    result->entry_capacity = 0U;

    for (i = 0U; i < pair_count; ++i) {
        if (pairs[i].key == NULL ||
            pairs[i].value == NULL ||
            dictionary_add(result, pairs[i].key, pairs[i].value) != 0) {
            dictionary_destroy(result);
            return -1;
        }
    }

    return 0;
}

static int dictionary_print(const Dictionary *dictionary)
{
    size_t i;
    size_t j;

    if (dictionary == NULL) {
        return -1;
    }

    for (i = 0U; i < dictionary->entry_count; ++i) {
        if (printf("%s: [", dictionary->entries[i].key) < 0) {
            return -1;
        }

        for (j = 0U; j < dictionary->entries[i].value_count; ++j) {
            if (printf("%s%s",
                       j == 0U ? "" : ", ",
                       dictionary->entries[i].values[j]) < 0) {
                return -1;
            }
        }

        if (printf("]\n") < 0) {
            return -1;
        }
    }

    return 0;
}

int main(void)
{
    const KeyValuePair pairs[] = {
        {"fruit", "apple"},
        {"color", "red"},
        {"fruit", "banana"},
        {"color", "blue"},
        {"fruit", "orange"}
    };
    Dictionary dictionary;
    size_t pair_count = sizeof(pairs) / sizeof(pairs[0]);
    int status = EXIT_SUCCESS;

    if (group_pairs(pairs, pair_count, &dictionary) != 0) {
        fprintf(stderr, "Failed to group key-value pairs.\n");
        return EXIT_FAILURE;
    }

    if (dictionary_print(&dictionary) != 0) {
        fprintf(stderr, "Failed to print dictionary.\n");
        status = EXIT_FAILURE;
    }

    dictionary_destroy(&dictionary);
    return status;
}