#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Dictionary Dictionary;

typedef enum {
    VALUE_INTEGER,
    VALUE_STRING,
    VALUE_DICTIONARY
} ValueType;

typedef struct {
    char *key;
    ValueType type;
    union {
        long integer;
        char *string;
        Dictionary *dictionary;
    } value;
} Entry;

struct Dictionary {
    Entry *entries;
    size_t count;
};

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

static Dictionary *dictionary_create(size_t count)
{
    Dictionary *dictionary;

    if (count > SIZE_MAX / sizeof(Entry)) {
        return NULL;
    }

    dictionary = calloc(1U, sizeof(*dictionary));
    if (dictionary == NULL) {
        return NULL;
    }

    if (count != 0U) {
        dictionary->entries = calloc(count, sizeof(*dictionary->entries));
        if (dictionary->entries == NULL) {
            free(dictionary);
            return NULL;
        }
    }

    dictionary->count = count;
    return dictionary;
}

static void dictionary_destroy(Dictionary *dictionary)
{
    size_t index;

    if (dictionary == NULL) {
        return;
    }

    for (index = 0U; index < dictionary->count; ++index) {
        free(dictionary->entries[index].key);

        if (dictionary->entries[index].type == VALUE_STRING) {
            free(dictionary->entries[index].value.string);
        } else if (dictionary->entries[index].type == VALUE_DICTIONARY) {
            dictionary_destroy(dictionary->entries[index].value.dictionary);
        }
    }

    free(dictionary->entries);
    free(dictionary);
}

static int dictionary_set_integer(Dictionary *dictionary, size_t index,
                                  const char *key, long value)
{
    char *key_copy;

    if (dictionary == NULL || index >= dictionary->count || key == NULL) {
        return 0;
    }

    key_copy = duplicate_string(key);
    if (key_copy == NULL) {
        return 0;
    }

    free(dictionary->entries[index].key);
    dictionary->entries[index].key = key_copy;
    dictionary->entries[index].type = VALUE_INTEGER;
    dictionary->entries[index].value.integer = value;
    return 1;
}

static int dictionary_set_dictionary(Dictionary *dictionary, size_t index,
                                     const char *key, Dictionary *child)
{
    char *key_copy;

    if (dictionary == NULL || index >= dictionary->count ||
        key == NULL || child == NULL) {
        return 0;
    }

    key_copy = duplicate_string(key);
    if (key_copy == NULL) {
        return 0;
    }

    free(dictionary->entries[index].key);
    dictionary->entries[index].key = key_copy;
    dictionary->entries[index].type = VALUE_DICTIONARY;
    dictionary->entries[index].value.dictionary = child;
    return 1;
}

static size_t dictionary_depth(const Dictionary *dictionary)
{
    size_t index;
    size_t maximum_child_depth = 0U;

    if (dictionary == NULL) {
        return 0U;
    }

    for (index = 0U; index < dictionary->count; ++index) {
        if (dictionary->entries[index].type == VALUE_DICTIONARY) {
            size_t child_depth =
                dictionary_depth(dictionary->entries[index].value.dictionary);

            if (child_depth > maximum_child_depth) {
                maximum_child_depth = child_depth;
            }
        }
    }

    if (maximum_child_depth == SIZE_MAX) {
        return SIZE_MAX;
    }

    return maximum_child_depth + 1U;
}

int main(void)
{
    Dictionary *root = NULL;
    Dictionary *child = NULL;
    Dictionary *grandchild = NULL;
    size_t depth;
    int status = EXIT_FAILURE;

    root = dictionary_create(2U);
    child = dictionary_create(1U);
    grandchild = dictionary_create(1U);

    if (root == NULL || child == NULL || grandchild == NULL) {
        goto cleanup;
    }

    if (!dictionary_set_integer(grandchild, 0U, "value", 42L)) {
        goto cleanup;
    }

    if (!dictionary_set_dictionary(child, 0U, "grandchild", grandchild)) {
        goto cleanup;
    }
    grandchild = NULL;

    if (!dictionary_set_dictionary(root, 0U, "child", child)) {
        goto cleanup;
    }
    child = NULL;

    if (!dictionary_set_integer(root, 1U, "number", 7L)) {
        goto cleanup;
    }

    depth = dictionary_depth(root);
    if (printf("%zu\n", depth) < 0) {
        goto cleanup;
    }

    status = EXIT_SUCCESS;

cleanup:
    dictionary_destroy(grandchild);
    dictionary_destroy(child);
    dictionary_destroy(root);
    return status;
}