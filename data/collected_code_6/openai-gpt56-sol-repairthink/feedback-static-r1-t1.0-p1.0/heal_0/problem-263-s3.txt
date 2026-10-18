#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *key;
    char *value;
} Entry;

typedef struct {
    Entry *entries;
    size_t size;
    size_t capacity;
} Dictionary;

static void dictionary_init(Dictionary *dictionary)
{
    dictionary->entries = NULL;
    dictionary->size = 0;
    dictionary->capacity = 0;
}

static void dictionary_destroy(Dictionary *dictionary)
{
    size_t i;

    if (dictionary == NULL) {
        return;
    }

    for (i = 0; i < dictionary->size; ++i) {
        free(dictionary->entries[i].key);
        free(dictionary->entries[i].value);
    }

    free(dictionary->entries);
    dictionary_init(dictionary);
}

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

    copy = malloc(length + 1);
    if (copy == NULL) {
        return NULL;
    }

    memcpy(copy, source, length + 1);
    return copy;
}

static int dictionary_reserve(Dictionary *dictionary, size_t capacity)
{
    Entry *entries;

    if (dictionary == NULL) {
        return -1;
    }

    if (capacity <= dictionary->capacity) {
        return 0;
    }

    if (capacity > SIZE_MAX / sizeof(*entries)) {
        return -1;
    }

    entries = realloc(dictionary->entries, capacity * sizeof(*entries));
    if (entries == NULL) {
        return -1;
    }

    dictionary->entries = entries;
    dictionary->capacity = capacity;
    return 0;
}

static size_t dictionary_find(const Dictionary *dictionary, const char *key)
{
    size_t i;

    if (dictionary == NULL || key == NULL) {
        return SIZE_MAX;
    }

    for (i = 0; i < dictionary->size; ++i) {
        if (strcmp(dictionary->entries[i].key, key) == 0) {
            return i;
        }
    }

    return SIZE_MAX;
}

static int dictionary_set(Dictionary *dictionary, const char *key,
                          const char *value)
{
    size_t index;
    size_t new_capacity;
    char *key_copy;
    char *value_copy;

    if (dictionary == NULL || key == NULL || value == NULL) {
        return -1;
    }

    index = dictionary_find(dictionary, key);
    value_copy = duplicate_string(value);
    if (value_copy == NULL) {
        return -1;
    }

    if (index != SIZE_MAX) {
        free(dictionary->entries[index].value);
        dictionary->entries[index].value = value_copy;
        return 0;
    }

    key_copy = duplicate_string(key);
    if (key_copy == NULL) {
        free(value_copy);
        return -1;
    }

    if (dictionary->size == dictionary->capacity) {
        if (dictionary->capacity == 0) {
            new_capacity = 4;
        } else {
            if (dictionary->capacity > SIZE_MAX / 2) {
                free(key_copy);
                free(value_copy);
                return -1;
            }
            new_capacity = dictionary->capacity * 2;
        }

        if (dictionary_reserve(dictionary, new_capacity) != 0) {
            free(key_copy);
            free(value_copy);
            return -1;
        }
    }

    dictionary->entries[dictionary->size].key = key_copy;
    dictionary->entries[dictionary->size].value = value_copy;
    ++dictionary->size;
    return 0;
}

static int dictionary_copy(Dictionary *destination,
                           const Dictionary *source)
{
    size_t i;
    Dictionary copy;

    if (destination == NULL || source == NULL) {
        return -1;
    }

    dictionary_init(&copy);

    if (dictionary_reserve(&copy, source->size) != 0) {
        return -1;
    }

    for (i = 0; i < source->size; ++i) {
        if (dictionary_set(&copy, source->entries[i].key,
                           source->entries[i].value) != 0) {
            dictionary_destroy(&copy);
            return -1;
        }
    }

    *destination = copy;
    return 0;
}

static int dictionary_merge(Dictionary *result, const Dictionary *first,
                            const Dictionary *second)
{
    size_t i;
    Dictionary merged;

    if (result == NULL || first == NULL || second == NULL ||
        result == first || result == second) {
        return -1;
    }

    if (dictionary_copy(&merged, first) != 0) {
        return -1;
    }

    for (i = 0; i < second->size; ++i) {
        if (dictionary_set(&merged, second->entries[i].key,
                           second->entries[i].value) != 0) {
            dictionary_destroy(&merged);
            return -1;
        }
    }

    dictionary_destroy(result);
    *result = merged;
    return 0;
}

static void dictionary_print(const Dictionary *dictionary)
{
    size_t i;

    if (dictionary == NULL) {
        return;
    }

    for (i = 0; i < dictionary->size; ++i) {
        printf("%s: %s\n", dictionary->entries[i].key,
               dictionary->entries[i].value);
    }
}

int main(void)
{
    Dictionary first;
    Dictionary second;
    Dictionary merged;
    int status = EXIT_FAILURE;

    dictionary_init(&first);
    dictionary_init(&second);
    dictionary_init(&merged);

    if (dictionary_set(&first, "name", "Alice") != 0 ||
        dictionary_set(&first, "city", "London") != 0 ||
        dictionary_set(&second, "city", "Paris") != 0 ||
        dictionary_set(&second, "role", "Developer") != 0) {
        fputs("Failed to create dictionaries.\n", stderr);
        goto cleanup;
    }

    if (dictionary_merge(&merged, &first, &second) != 0) {
        fputs("Failed to merge dictionaries.\n", stderr);
        goto cleanup;
    }

    dictionary_print(&merged);
    status = EXIT_SUCCESS;

cleanup:
    dictionary_destroy(&merged);
    dictionary_destroy(&second);
    dictionary_destroy(&first);
    return status;
}