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

static int dictionary_keys(const Dictionary *dictionary,
                           char ***keys_out,
                           size_t *count_out)
{
    char **keys;
    size_t i;

    if (dictionary == NULL || keys_out == NULL || count_out == NULL) {
        return -1;
    }

    *keys_out = NULL;
    *count_out = 0U;

    if (dictionary->count == 0U) {
        return 0;
    }

    if (dictionary->entries == NULL ||
        dictionary->count > SIZE_MAX / sizeof(*keys)) {
        return -1;
    }

    keys = calloc(dictionary->count, sizeof(*keys));
    if (keys == NULL) {
        return -1;
    }

    for (i = 0U; i < dictionary->count; ++i) {
        if (dictionary->entries[i].key == NULL) {
            while (i > 0U) {
                --i;
                free(keys[i]);
            }
            free(keys);
            return -1;
        }

        keys[i] = duplicate_string(dictionary->entries[i].key);
        if (keys[i] == NULL) {
            while (i > 0U) {
                --i;
                free(keys[i]);
            }
            free(keys);
            return -1;
        }
    }

    *keys_out = keys;
    *count_out = dictionary->count;
    return 0;
}

static void free_string_list(char **list, size_t count)
{
    size_t i;

    if (list == NULL) {
        return;
    }

    for (i = 0U; i < count; ++i) {
        free(list[i]);
    }

    free(list);
}

int main(void)
{
    DictionaryEntry entries[] = {
        {"name", "Alice"},
        {"language", "C"},
        {"level", "experienced"}
    };
    Dictionary dictionary = {
        entries,
        sizeof(entries) / sizeof(entries[0])
    };
    char **keys = NULL;
    size_t key_count = 0U;
    size_t i;

    if (dictionary_keys(&dictionary, &keys, &key_count) != 0) {
        fputs("Failed to retrieve dictionary keys.\n", stderr);
        return EXIT_FAILURE;
    }

    for (i = 0U; i < key_count; ++i) {
        if (puts(keys[i]) == EOF) {
            free_string_list(keys, key_count);
            return EXIT_FAILURE;
        }
    }

    free_string_list(keys, key_count);
    return EXIT_SUCCESS;
}