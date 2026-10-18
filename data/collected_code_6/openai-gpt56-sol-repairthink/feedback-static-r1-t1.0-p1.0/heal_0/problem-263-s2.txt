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

static int dictionary_init(Dictionary *dictionary)
{
    if (dictionary == NULL) {
        return 0;
    }

    dictionary->entries = NULL;
    dictionary->size = 0U;
    dictionary->capacity = 0U;
    return 1;
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
    dictionary->capacity = 0U;
}

static size_t dictionary_find(const Dictionary *dictionary, const char *key)
{
    size_t index;

    if (dictionary == NULL || key == NULL) {
        return SIZE_MAX;
    }

    for (index = 0U; index < dictionary->size; ++index) {
        if (strcmp(dictionary->entries[index].key, key) == 0) {
            return index;
        }
    }

    return SIZE_MAX;
}

static int dictionary_reserve(Dictionary *dictionary, size_t capacity)
{
    DictionaryEntry *new_entries;

    if (dictionary == NULL) {
        return 0;
    }

    if (capacity <= dictionary->capacity) {
        return 1;
    }

    if (capacity > SIZE_MAX / sizeof(*new_entries)) {
        return 0;
    }

    new_entries = realloc(dictionary->entries, capacity * sizeof(*new_entries));
    if (new_entries == NULL) {
        return 0;
    }

    dictionary->entries = new_entries;
    dictionary->capacity = capacity;
    return 1;
}

static int dictionary_set(Dictionary *dictionary, const char *key, const char *value)
{
    size_t index;
    size_t new_capacity;
    char *new_key;
    char *new_value;

    if (dictionary == NULL || key == NULL || value == NULL) {
        return 0;
    }

    index = dictionary_find(dictionary, key);
    if (index != SIZE_MAX) {
        new_value = duplicate_string(value);
        if (new_value == NULL) {
            return 0;
        }

        free(dictionary->entries[index].value);
        dictionary->entries[index].value = new_value;
        return 1;
    }

    if (dictionary->size == dictionary->capacity) {
        if (dictionary->capacity == 0U) {
            new_capacity = 4U;
        } else {
            if (dictionary->capacity > SIZE_MAX / 2U) {
                return 0;
            }
            new_capacity = dictionary->capacity * 2U;
        }

        if (!dictionary_reserve(dictionary, new_capacity)) {
            return 0;
        }
    }

    new_key = duplicate_string(key);
    if (new_key == NULL) {
        return 0;
    }

    new_value = duplicate_string(value);
    if (new_value == NULL) {
        free(new_key);
        return 0;
    }

    dictionary->entries[dictionary->size].key = new_key;
    dictionary->entries[dictionary->size].value = new_value;
    ++dictionary->size;
    return 1;
}

static int dictionary_merge(
    const Dictionary *first,
    const Dictionary *second,
    Dictionary *result)
{
    Dictionary temporary;
    size_t index;

    if (first == NULL || second == NULL || result == NULL ||
        result == first || result == second) {
        return 0;
    }

    if (!dictionary_init(&temporary)) {
        return 0;
    }

    if (first->size > SIZE_MAX - second->size ||
        !dictionary_reserve(&temporary, first->size + second->size)) {
        dictionary_destroy(&temporary);
        return 0;
    }

    for (index = 0U; index < first->size; ++index) {
        if (!dictionary_set(
                &temporary,
                first->entries[index].key,
                first->entries[index].value)) {
            dictionary_destroy(&temporary);
            return 0;
        }
    }

    for (index = 0U; index < second->size; ++index) {
        if (!dictionary_set(
                &temporary,
                second->entries[index].key,
                second->entries[index].value)) {
            dictionary_destroy(&temporary);
            return 0;
        }
    }

    dictionary_destroy(result);
    *result = temporary;
    return 1;
}

static int dictionary_print(const Dictionary *dictionary)
{
    size_t index;

    if (dictionary == NULL) {
        return 0;
    }

    for (index = 0U; index < dictionary->size; ++index) {
        if (printf("%s: %s\n",
                   dictionary->entries[index].key,
                   dictionary->entries[index].value) < 0) {
            return 0;
        }
    }

    return 1;
}

int main(void)
{
    Dictionary first;
    Dictionary second;
    Dictionary merged;
    int exit_status = EXIT_FAILURE;

    if (!dictionary_init(&first) ||
        !dictionary_init(&second) ||
        !dictionary_init(&merged)) {
        return EXIT_FAILURE;
    }

    if (!dictionary_set(&first, "name", "Alice") ||
        !dictionary_set(&first, "city", "London") ||
        !dictionary_set(&second, "city", "Paris") ||
        !dictionary_set(&second, "language", "C")) {
        goto cleanup;
    }

    if (!dictionary_merge(&first, &second, &merged)) {
        goto cleanup;
    }

    if (!dictionary_print(&merged)) {
        goto cleanup;
    }

    exit_status = EXIT_SUCCESS;

cleanup:
    dictionary_destroy(&merged);
    dictionary_destroy(&second);
    dictionary_destroy(&first);
    return exit_status;
}