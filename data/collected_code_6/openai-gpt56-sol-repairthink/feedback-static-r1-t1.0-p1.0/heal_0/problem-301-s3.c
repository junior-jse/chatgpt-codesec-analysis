#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Dictionary Dictionary;

typedef enum {
    VALUE_STRING,
    VALUE_DICTIONARY
} ValueType;

typedef struct {
    ValueType type;
    union {
        char *string;
        Dictionary *dictionary;
    } data;
} Value;

typedef struct {
    char *key;
    Value value;
} Entry;

struct Dictionary {
    Entry *entries;
    size_t count;
};

static char *duplicate_string(const char *source)
{
    char *copy;
    size_t length;

    if (source == NULL) {
        return NULL;
    }

    length = strlen(source);
    copy = malloc(length + 1U);
    if (copy == NULL) {
        return NULL;
    }

    memcpy(copy, source, length + 1U);
    return copy;
}

static Dictionary *dictionary_create(size_t count)
{
    Dictionary *dictionary;

    dictionary = calloc(1U, sizeof(*dictionary));
    if (dictionary == NULL) {
        return NULL;
    }

    if (count > 0U) {
        dictionary->entries = calloc(count, sizeof(*dictionary->entries));
        if (dictionary->entries == NULL) {
            free(dictionary);
            return NULL;
        }
    }

    dictionary->count = count;
    return dictionary;
}

static int dictionary_set_string(
    Dictionary *dictionary,
    size_t index,
    const char *key,
    const char *value)
{
    char *key_copy;
    char *value_copy;

    if (dictionary == NULL || key == NULL || value == NULL ||
        index >= dictionary->count || dictionary->entries[index].key != NULL) {
        return -1;
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

    dictionary->entries[index].key = key_copy;
    dictionary->entries[index].value.type = VALUE_STRING;
    dictionary->entries[index].value.data.string = value_copy;
    return 0;
}

static int dictionary_set_dictionary(
    Dictionary *dictionary,
    size_t index,
    const char *key,
    Dictionary *child)
{
    char *key_copy;

    if (dictionary == NULL || key == NULL || child == NULL ||
        index >= dictionary->count || dictionary->entries[index].key != NULL) {
        return -1;
    }

    key_copy = duplicate_string(key);
    if (key_copy == NULL) {
        return -1;
    }

    dictionary->entries[index].key = key_copy;
    dictionary->entries[index].value.type = VALUE_DICTIONARY;
    dictionary->entries[index].value.data.dictionary = child;
    return 0;
}

static size_t dictionary_depth(const Dictionary *dictionary)
{
    size_t maximum_child_depth = 0U;
    size_t index;

    if (dictionary == NULL) {
        return 0U;
    }

    for (index = 0U; index < dictionary->count; ++index) {
        const Entry *entry = &dictionary->entries[index];

        if (entry->key != NULL &&
            entry->value.type == VALUE_DICTIONARY &&
            entry->value.data.dictionary != NULL) {
            size_t child_depth =
                dictionary_depth(entry->value.data.dictionary);

            if (child_depth > maximum_child_depth) {
                maximum_child_depth = child_depth;
            }
        }
    }

    return maximum_child_depth + 1U;
}

static void dictionary_destroy(Dictionary *dictionary)
{
    size_t index;

    if (dictionary == NULL) {
        return;
    }

    for (index = 0U; index < dictionary->count; ++index) {
        Entry *entry = &dictionary->entries[index];

        free(entry->key);

        if (entry->value.type == VALUE_STRING) {
            free(entry->value.data.string);
        } else if (entry->value.type == VALUE_DICTIONARY) {
            dictionary_destroy(entry->value.data.dictionary);
        }
    }

    free(dictionary->entries);
    free(dictionary);
}

int main(void)
{
    Dictionary *root = NULL;
    Dictionary *level_one = NULL;
    Dictionary *level_two = NULL;
    int status = EXIT_FAILURE;

    root = dictionary_create(2U);
    level_one = dictionary_create(1U);
    level_two = dictionary_create(1U);

    if (root == NULL || level_one == NULL || level_two == NULL) {
        goto cleanup;
    }

    if (dictionary_set_string(level_two, 0U, "name", "leaf") != 0) {
        goto cleanup;
    }

    if (dictionary_set_dictionary(level_one, 0U, "nested", level_two) != 0) {
        goto cleanup;
    }
    level_two = NULL;

    if (dictionary_set_dictionary(root, 0U, "child", level_one) != 0) {
        goto cleanup;
    }
    level_one = NULL;

    if (dictionary_set_string(root, 1U, "title", "example") != 0) {
        goto cleanup;
    }

    if (printf("%zu\n", dictionary_depth(root)) < 0) {
        goto cleanup;
    }

    status = EXIT_SUCCESS;

cleanup:
    dictionary_destroy(level_two);
    dictionary_destroy(level_one);
    dictionary_destroy(root);
    return status;
}