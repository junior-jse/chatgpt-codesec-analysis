#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *key;
    char *value;
} KeyValuePair;

typedef struct {
    char *key;
    char **values;
    size_t value_count;
    size_t value_capacity;
} DictionaryEntry;

typedef struct {
    DictionaryEntry *entries;
    size_t count;
    size_t capacity;
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

static void free_dictionary(Dictionary *dictionary)
{
    size_t i;
    size_t j;

    if (dictionary == NULL) {
        return;
    }

    for (i = 0U; i < dictionary->count; ++i) {
        free(dictionary->entries[i].key);
        for (j = 0U; j < dictionary->entries[i].value_count; ++j) {
            free(dictionary->entries[i].values[j]);
        }
        free(dictionary->entries[i].values);
    }

    free(dictionary->entries);
    dictionary->entries = NULL;
    dictionary->count = 0U;
    dictionary->capacity = 0U;
}

static int grow_entries(Dictionary *dictionary)
{
    size_t new_capacity;
    DictionaryEntry *new_entries;

    if (dictionary->count < dictionary->capacity) {
        return 0;
    }

    if (dictionary->capacity == 0U) {
        new_capacity = 4U;
    } else {
        if (dictionary->capacity > SIZE_MAX / 2U) {
            return -1;
        }
        new_capacity = dictionary->capacity * 2U;
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
    dictionary->capacity = new_capacity;
    return 0;
}

static int append_value(DictionaryEntry *entry, const char *value)
{
    size_t new_capacity;
    char **new_values;
    char *value_copy;

    if (entry->value_count == entry->value_capacity) {
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
    }

    value_copy = duplicate_string(value);
    if (value_copy == NULL) {
        return -1;
    }

    entry->values[entry->value_count] = value_copy;
    ++entry->value_count;
    return 0;
}

static DictionaryEntry *find_entry(Dictionary *dictionary, const char *key)
{
    size_t i;

    for (i = 0U; i < dictionary->count; ++i) {
        if (strcmp(dictionary->entries[i].key, key) == 0) {
            return &dictionary->entries[i];
        }
    }

    return NULL;
}

static int add_pair(Dictionary *dictionary, const char *key, const char *value)
{
    DictionaryEntry *entry;
    char *key_copy;

    entry = find_entry(dictionary, key);
    if (entry != NULL) {
        return append_value(entry, value);
    }

    if (grow_entries(dictionary) != 0) {
        return -1;
    }

    key_copy = duplicate_string(key);
    if (key_copy == NULL) {
        return -1;
    }

    entry = &dictionary->entries[dictionary->count];
    entry->key = key_copy;
    entry->values = NULL;
    entry->value_count = 0U;
    entry->value_capacity = 0U;

    if (append_value(entry, value) != 0) {
        free(entry->key);
        entry->key = NULL;
        return -1;
    }

    ++dictionary->count;
    return 0;
}

static int group_pairs(const KeyValuePair *pairs, size_t pair_count,
                       Dictionary *result)
{
    size_t i;

    if (result == NULL || (pairs == NULL && pair_count != 0U)) {
        return -1;
    }

    result->entries = NULL;
    result->count = 0U;
    result->capacity = 0U;

    for (i = 0U; i < pair_count; ++i) {
        if (pairs[i].key == NULL || pairs[i].value == NULL ||
            add_pair(result, pairs[i].key, pairs[i].value) != 0) {
            free_dictionary(result);
            return -1;
        }
    }

    return 0;
}

static int print_dictionary(const Dictionary *dictionary)
{
    size_t i;
    size_t j;

    if (dictionary == NULL) {
        return -1;
    }

    for (i = 0U; i < dictionary->count; ++i) {
        if (printf("%s: [", dictionary->entries[i].key) < 0) {
            return -1;
        }

        for (j = 0U; j < dictionary->entries[i].value_count; ++j) {
            if (printf("%s\"%s\"", j == 0U ? "" : ", ",
                       dictionary->entries[i].values[j]) < 0) {
                return -1;
            }
        }

        if (puts("]") == EOF) {
            return -1;
        }
    }

    return 0;
}

int main(void)
{
    KeyValuePair pairs[] = {
        {"fruit", "apple"},
        {"color", "red"},
        {"fruit", "banana"},
        {"color", "blue"},
        {"fruit", "orange"},
        {"shape", "circle"}
    };
    Dictionary dictionary;
    size_t pair_count = sizeof(pairs) / sizeof(pairs[0]);

    if (group_pairs(pairs, pair_count, &dictionary) != 0) {
        fputs("Failed to group key-value pairs.\n", stderr);
        return EXIT_FAILURE;
    }

    if (print_dictionary(&dictionary) != 0) {
        fputs("Failed to print dictionary.\n", stderr);
        free_dictionary(&dictionary);
        return EXIT_FAILURE;
    }

    free_dictionary(&dictionary);
    return EXIT_SUCCESS;
}