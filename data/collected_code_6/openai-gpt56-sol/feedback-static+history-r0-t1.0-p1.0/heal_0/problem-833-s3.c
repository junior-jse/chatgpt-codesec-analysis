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

    copy = malloc(length + 1);
    if (copy == NULL) {
        return NULL;
    }

    memcpy(copy, source, length + 1);
    return copy;
}

static char **dictionary_keys(const Dictionary *dictionary, size_t *key_count)
{
    char **keys;
    size_t i;

    if (key_count == NULL) {
        return NULL;
    }

    *key_count = 0;

    if (dictionary == NULL ||
        (dictionary->count > 0 && dictionary->entries == NULL)) {
        return NULL;
    }

    if (dictionary->count == 0) {
        return calloc(1, sizeof(*keys));
    }

    if (dictionary->count > (SIZE_MAX / sizeof(*keys)) - 1) {
        return NULL;
    }

    keys = calloc(dictionary->count + 1, sizeof(*keys));
    if (keys == NULL) {
        return NULL;
    }

    for (i = 0; i < dictionary->count; ++i) {
        if (dictionary->entries[i].key == NULL) {
            while (i > 0) {
                free(keys[--i]);
            }
            free(keys);
            return NULL;
        }

        keys[i] = duplicate_string(dictionary->entries[i].key);
        if (keys[i] == NULL) {
            while (i > 0) {
                free(keys[--i]);
            }
            free(keys);
            return NULL;
        }
    }

    *key_count = dictionary->count;
    return keys;
}

static void free_key_list(char **keys, size_t key_count)
{
    size_t i;

    if (keys == NULL) {
        return;
    }

    for (i = 0; i < key_count; ++i) {
        free(keys[i]);
    }

    free(keys);
}

int main(void)
{
    DictionaryEntry entries[] = {
        {"name", "Alice"},
        {"age", "30"},
        {"city", "London"}
    };
    Dictionary dictionary = {
        entries,
        sizeof(entries) / sizeof(entries[0])
    };
    char **keys;
    size_t key_count;
    size_t i;

    keys = dictionary_keys(&dictionary, &key_count);
    if (keys == NULL) {
        fputs("Failed to retrieve dictionary keys.\n", stderr);
        return EXIT_FAILURE;
    }

    for (i = 0; i < key_count; ++i) {
        if (printf("%s\n", keys[i]) < 0) {
            free_key_list(keys, key_count);
            return EXIT_FAILURE;
        }
    }

    free_key_list(keys, key_count);
    return EXIT_SUCCESS;
}