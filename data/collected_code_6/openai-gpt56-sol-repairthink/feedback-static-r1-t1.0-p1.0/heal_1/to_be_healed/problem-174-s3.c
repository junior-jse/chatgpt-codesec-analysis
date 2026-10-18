#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>

typedef struct {
    const char *key;
    const char *value;
} KeyValuePair;

typedef struct {
    char **items;
    size_t count;
    size_t capacity;
} StringList;

typedef struct {
    char *key;
    StringList values;
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

static void string_list_destroy(StringList *list)
{
    size_t i;

    if (list == NULL) {
        return;
    }

    for (i = 0U; i < list->count; ++i) {
        free(list->items[i]);
        list->items[i] = NULL;
    }

    free(list->items);
    list->items = NULL;
    list->count = 0U;
    list->capacity = 0U;
}

static void dictionary_destroy(Dictionary *dictionary)
{
    size_t i;

    if (dictionary == NULL) {
        return;
    }

    for (i = 0U; i < dictionary->count; ++i) {
        free(dictionary->entries[i].key);
        dictionary->entries[i].key = NULL;
        string_list_destroy(&dictionary->entries[i].values);
    }

    free(dictionary->entries);
    dictionary->entries = NULL;
    dictionary->count = 0U;
    dictionary->capacity = 0U;
}

static int string_list_append(StringList *list, const char *value)
{
    char *copy;

    if (list == NULL || value == NULL) {
        errno = EINVAL;
        return -1;
    }

    copy = duplicate_string(value);
    if (copy == NULL) {
        errno = ENOMEM;
        return -1;
    }

    if (list->count == list->capacity) {
        size_t new_capacity;
        char **new_items;

        if (list->capacity == 0U) {
            new_capacity = 4U;
        } else {
            if (list->capacity > SIZE_MAX / 2U) {
                free(copy);
                errno = EOVERFLOW;
                return -1;
            }
            new_capacity = list->capacity * 2U;
        }

        if (new_capacity > SIZE_MAX / sizeof(*new_items)) {
            free(copy);
            errno = EOVERFLOW;
            return -1;
        }

        new_items = realloc(list->items, new_capacity * sizeof(*new_items));
        if (new_items == NULL) {
            free(copy);
            errno = ENOMEM;
            return -1;
        }

        list->items = new_items;
        list->capacity = new_capacity;
    }

    list->items[list->count] = copy;
    ++list->count;
    return 0;
}

static DictionaryEntry *dictionary_find(Dictionary *dictionary, const char *key)
{
    size_t i;

    if (dictionary == NULL || key == NULL) {
        return NULL;
    }

    for (i = 0U; i < dictionary->count; ++i) {
        if (strcmp(dictionary->entries[i].key, key) == 0) {
            return &dictionary->entries[i];
        }
    }

    return NULL;
}

static DictionaryEntry *dictionary_add_entry(Dictionary *dictionary,
                                              const char *key)
{
    DictionaryEntry *entry;
    char *key_copy;

    if (dictionary == NULL || key == NULL) {
        errno = EINVAL;
        return NULL;
    }

    key_copy = duplicate_string(key);
    if (key_copy == NULL) {
        errno = ENOMEM;
        return NULL;
    }

    if (dictionary->count == dictionary->capacity) {
        size_t new_capacity;
        DictionaryEntry *new_entries;

        if (dictionary->capacity == 0U) {
            new_capacity = 4U;
        } else {
            if (dictionary->capacity > SIZE_MAX / 2U) {
                free(key_copy);
                errno = EOVERFLOW;
                return NULL;
            }
            new_capacity = dictionary->capacity * 2U;
        }

        if (new_capacity > SIZE_MAX / sizeof(*new_entries)) {
            free(key_copy);
            errno = EOVERFLOW;
            return NULL;
        }

        new_entries = realloc(dictionary->entries,
                              new_capacity * sizeof(*new_entries));
        if (new_entries == NULL) {
            free(key_copy);
            errno = ENOMEM;
            return NULL;
        }

        dictionary->entries = new_entries;
        dictionary->capacity = new_capacity;
    }

    entry = &dictionary->entries[dictionary->count];
    entry->key = key_copy;
    entry->values.items = NULL;
    entry->values.count = 0U;
    entry->values.capacity = 0U;
    ++dictionary->count;

    return entry;
}

static int group_pairs(const KeyValuePair *pairs,
                       size_t pair_count,
                       Dictionary *result)
{
    size_t i;

    if (result == NULL || (pairs == NULL && pair_count != 0U)) {
        errno = EINVAL;
        return -1;
    }

    result->entries = NULL;
    result->count = 0U;
    result->capacity = 0U;

    for (i = 0U; i < pair_count; ++i) {
        DictionaryEntry *entry;
        int added_entry = 0;

        if (pairs[i].key == NULL || pairs[i].value == NULL) {
            errno = EINVAL;
            dictionary_destroy(result);
            return -1;
        }

        entry = dictionary_find(result, pairs[i].key);
        if (entry == NULL) {
            entry = dictionary_add_entry(result, pairs[i].key);
            if (entry == NULL) {
                dictionary_destroy(result);
                return -1;
            }
            added_entry = 1;
        }

        if (string_list_append(&entry->values, pairs[i].value) != 0) {
            if (added_entry != 0) {
                free(entry->key);
                entry->key = NULL;
                string_list_destroy(&entry->values);
                --result->count;
            }
            dictionary_destroy(result);
            return -1;
        }
    }

    return 0;
}

static int dictionary_print(const Dictionary *dictionary)
{
    size_t i;

    if (dictionary == NULL) {
        errno = EINVAL;
        return -1;
    }

    if (fputs("{\n", stdout) == EOF) {
        return -1;
    }

    for (i = 0U; i < dictionary->count; ++i) {
        size_t j;
        const DictionaryEntry *entry = &dictionary->entries[i];

        if (printf("  \"%s\": [", entry->key) < 0) {
            return -1;
        }

        for (j = 0U; j < entry->values.count; ++j) {
            if (printf("%s\"%s\"",
                       j == 0U ? "" : ", ",
                       entry->values.items[j]) < 0) {
                return -1;
            }
        }

        if (printf("]%s\n", i + 1U < dictionary->count ? "," : "") < 0) {
            return -1;
        }
    }

    if (fputs("}\n", stdout) == EOF) {
        return -1;
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
        {"fruit", "orange"},
        {"size", "large"}
    };
    Dictionary dictionary;
    int status = EXIT_SUCCESS;

    if (group_pairs(pairs, sizeof(pairs) / sizeof(pairs[0]), &dictionary) != 0) {
        perror("group_pairs");
        return EXIT_FAILURE;
    }

    if (dictionary_print(&dictionary) != 0) {
        perror("dictionary_print");
        status = EXIT_FAILURE;
    }

    dictionary_destroy(&dictionary);

    if (fflush(stdout) == EOF) {
        perror("fflush");
        status = EXIT_FAILURE;
    }

    return status;
}