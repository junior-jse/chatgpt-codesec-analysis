#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *key;
    /* Possible weaknesses found:
     *  struct member 'DictionaryEntry::value' is never used. [unusedStructMember]
     */
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

static void free_string_list(char **list, size_t count)
{
    size_t index;

    if (list == NULL) {
        return;
    }

    for (index = 0U; index < count; ++index) {
        free(list[index]);
    }

    free(list);
}

static int dictionary_get_keys(
    const Dictionary *dictionary,
    char ***keys,
    size_t *key_count)
{
    char **result;
    size_t index;

    if (dictionary == NULL || keys == NULL || key_count == NULL) {
        return -1;
    }

    *keys = NULL;
    *key_count = 0U;

    if (dictionary->count == 0U) {
        return 0;
    }

    if (dictionary->entries == NULL ||
        dictionary->count > SIZE_MAX / sizeof(*result)) {
        return -1;
    }

    result = calloc(dictionary->count, sizeof(*result));
    if (result == NULL) {
        return -1;
    }

    for (index = 0U; index < dictionary->count; ++index) {
        if (dictionary->entries[index].key == NULL) {
            free_string_list(result, index);
            return -1;
        }

        result[index] = duplicate_string(dictionary->entries[index].key);
        if (result[index] == NULL) {
            free_string_list(result, index);
            return -1;
        }
    }

    *keys = result;
    *key_count = dictionary->count;
    return 0;
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
    char **keys = NULL;
    size_t key_count = 0U;
    size_t index;

    if (dictionary_get_keys(&dictionary, &keys, &key_count) != 0) {
        fputs("Failed to retrieve dictionary keys.\n", stderr);
        return EXIT_FAILURE;
    }

    for (index = 0U; index < key_count; ++index) {
        if (printf("%s\n", keys[index]) < 0) {
            free_string_list(keys, key_count);
            return EXIT_FAILURE;
        }
    }

    free_string_list(keys, key_count);

    if (ferror(stdout) != 0) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}