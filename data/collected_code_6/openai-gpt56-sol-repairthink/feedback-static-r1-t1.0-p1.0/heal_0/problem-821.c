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
    size_t i;

    if (dictionary == NULL) {
        return;
    }

    for (i = 0U; i < dictionary->size; ++i) {
        free(dictionary->entries[i].key);
        free(dictionary->entries[i].value);
    }

    free(dictionary->entries);
    dictionary->entries = NULL;
    dictionary->size = 0U;
    dictionary->capacity = 0U;
}

static int dictionary_initialize(Dictionary *dictionary, size_t capacity)
{
    if (dictionary == NULL) {
        return -1;
    }

    dictionary->entries = NULL;
    dictionary->size = 0U;
    dictionary->capacity = 0U;

    if (capacity == 0U) {
        return 0;
    }

    if (capacity > SIZE_MAX / sizeof(*dictionary->entries)) {
        return -1;
    }

    dictionary->entries = calloc(capacity, sizeof(*dictionary->entries));
    if (dictionary->entries == NULL) {
        return -1;
    }

    dictionary->capacity = capacity;
    return 0;
}

static int dictionary_reserve(Dictionary *dictionary, size_t capacity)
{
    Entry *new_entries;

    if (dictionary == NULL) {
        return -1;
    }

    if (capacity <= dictionary->capacity) {
        return 0;
    }

    if (capacity > SIZE_MAX / sizeof(*dictionary->entries)) {
        return -1;
    }

    new_entries = realloc(dictionary->entries,
                          capacity * sizeof(*dictionary->entries));
    if (new_entries == NULL) {
        return -1;
    }

    dictionary->entries = new_entries;
    dictionary->capacity = capacity;
    return 0;
}

static size_t dictionary_find(const Dictionary *dictionary, const char *key)
{
    size_t i;

    if (dictionary == NULL || key == NULL) {
        return SIZE_MAX;
    }

    for (i = 0U; i < dictionary->size; ++i) {
        if (strcmp(dictionary->entries[i].key, key) == 0) {
            return i;
        }
    }

    return SIZE_MAX;
}

static int dictionary_set(Dictionary *dictionary,
                          const char *key,
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
    if (index != SIZE_MAX) {
        value_copy = duplicate_string(value);
        if (value_copy == NULL) {
            return -1;
        }

        free(dictionary->entries[index].value);
        dictionary->entries[index].value = value_copy;
        return 0;
    }

    if (dictionary->size == dictionary->capacity) {
        if (dictionary->capacity == 0U) {
            new_capacity = 4U;
        } else {
            if (dictionary->capacity > SIZE_MAX / 2U) {
                return -1;
            }
            new_capacity = dictionary->capacity * 2U;
        }

        if (dictionary_reserve(dictionary, new_capacity) != 0) {
            return -1;
        }
    }

    key_copy = duplicate_string(key);
    if (key_copy == NULL) {
        return -1;
    }

    value_copy = duplicate_string(value);
    if (value_copy == NULL) {
        free(key_copy);
        return -1;
    }

    dictionary->entries[dictionary->size].key = key_copy;
    dictionary->entries[dictionary->size].value = value_copy;
    ++dictionary->size;

    return 0;
}

static int dictionary_merge(const Dictionary *first,
                            const Dictionary *second,
                            Dictionary *result)
{
    size_t i;
    size_t initial_capacity;

    if (first == NULL || second == NULL || result == NULL) {
        return -1;
    }

    if (first->size > SIZE_MAX - second->size) {
        return -1;
    }

    initial_capacity = first->size + second->size;
    if (dictionary_initialize(result, initial_capacity) != 0) {
        return -1;
    }

    for (i = 0U; i < first->size; ++i) {
        if (dictionary_set(result,
                           first->entries[i].key,
                           first->entries[i].value) != 0) {
            dictionary_destroy(result);
            return -1;
        }
    }

    for (i = 0U; i < second->size; ++i) {
        if (dictionary_set(result,
                           second->entries[i].key,
                           second->entries[i].value) != 0) {
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
        return -1;
    }

    if (putchar('{') == EOF) {
        return -1;
    }

    for (i = 0U; i < dictionary->size; ++i) {
        if (i != 0U && fputs(", ", stdout) == EOF) {
            return -1;
        }

        if (printf("\"%s\": \"%s\"",
                   dictionary->entries[i].key,
                   dictionary->entries[i].value) < 0) {
            return -1;
        }
    }

    if (puts("}") == EOF) {
        return -1;
    }

    return 0;
}

int main(void)
{
    Dictionary first;
    Dictionary second;
    Dictionary merged;
    int status = EXIT_FAILURE;

    if (dictionary_initialize(&first, 0U) != 0) {
        return EXIT_FAILURE;
    }

    if (dictionary_initialize(&second, 0U) != 0) {
        dictionary_destroy(&first);
        return EXIT_FAILURE;
    }

    if (dictionary_set(&first, "name", "Alice") != 0 ||
        dictionary_set(&first, "city", "Paris") != 0 ||
        dictionary_set(&second, "city", "London") != 0 ||
        dictionary_set(&second, "age", "30") != 0) {
        goto cleanup;
    }

    if (dictionary_merge(&first, &second, &merged) != 0) {
        goto cleanup;
    }

    if (dictionary_print(&merged) == 0) {
        status = EXIT_SUCCESS;
    }

    dictionary_destroy(&merged);

cleanup:
    dictionary_destroy(&second);
    dictionary_destroy(&first);
    return status;
}