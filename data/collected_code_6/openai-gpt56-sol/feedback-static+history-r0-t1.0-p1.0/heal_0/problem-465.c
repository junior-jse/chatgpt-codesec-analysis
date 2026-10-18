#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *key;
    char *value;
} DictionaryItem;

typedef struct {
    DictionaryItem *items;
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
    size_t index;

    if (dictionary == NULL) {
        return;
    }

    for (index = 0U; index < dictionary->size; ++index) {
        free(dictionary->items[index].key);
        free(dictionary->items[index].value);
    }

    free(dictionary->items);
    dictionary->items = NULL;
    dictionary->size = 0U;
    dictionary->capacity = 0U;
}

static bool dictionary_initialize(Dictionary *dictionary, size_t capacity)
{
    if (dictionary == NULL) {
        return false;
    }

    dictionary->items = NULL;
    dictionary->size = 0U;
    dictionary->capacity = 0U;

    if (capacity == 0U) {
        return true;
    }

    if (capacity > SIZE_MAX / sizeof(*dictionary->items)) {
        return false;
    }

    dictionary->items = calloc(capacity, sizeof(*dictionary->items));
    if (dictionary->items == NULL) {
        return false;
    }

    dictionary->capacity = capacity;
    return true;
}

static bool dictionary_add(Dictionary *dictionary, const char *key, const char *value)
{
    char *key_copy;
    char *value_copy;

    if (dictionary == NULL || key == NULL || value == NULL ||
        dictionary->size >= dictionary->capacity) {
        return false;
    }

    key_copy = duplicate_string(key);
    if (key_copy == NULL) {
        return false;
    }

    value_copy = duplicate_string(value);
    if (value_copy == NULL) {
        free(key_copy);
        return false;
    }

    dictionary->items[dictionary->size].key = key_copy;
    dictionary->items[dictionary->size].value = value_copy;
    ++dictionary->size;

    return true;
}

static bool is_empty_item(const DictionaryItem *item)
{
    return item == NULL || item->key == NULL || item->value == NULL ||
           item->key[0] == '\0' || item->value[0] == '\0';
}

static void dictionary_drop_empty(Dictionary *dictionary)
{
    size_t read_index;
    size_t write_index = 0U;

    if (dictionary == NULL || dictionary->items == NULL) {
        return;
    }

    for (read_index = 0U; read_index < dictionary->size; ++read_index) {
        if (is_empty_item(&dictionary->items[read_index])) {
            free(dictionary->items[read_index].key);
            free(dictionary->items[read_index].value);
        } else {
            if (write_index != read_index) {
                dictionary->items[write_index] = dictionary->items[read_index];
            }
            ++write_index;
        }
    }

    for (read_index = write_index; read_index < dictionary->size; ++read_index) {
        dictionary->items[read_index].key = NULL;
        dictionary->items[read_index].value = NULL;
    }

    dictionary->size = write_index;
}

static bool dictionary_print(const Dictionary *dictionary)
{
    size_t index;

    if (dictionary == NULL) {
        return false;
    }

    for (index = 0U; index < dictionary->size; ++index) {
        if (printf("%s: %s\n",
                   dictionary->items[index].key,
                   dictionary->items[index].value) < 0) {
            return false;
        }
    }

    return true;
}

int main(void)
{
    Dictionary dictionary;
    int exit_status = EXIT_FAILURE;

    if (!dictionary_initialize(&dictionary, 5U)) {
        return EXIT_FAILURE;
    }

    if (!dictionary_add(&dictionary, "name", "Alice") ||
        !dictionary_add(&dictionary, "email", "") ||
        !dictionary_add(&dictionary, "", "unused") ||
        !dictionary_add(&dictionary, "city", "London") ||
        !dictionary_add(&dictionary, "country", "UK")) {
        dictionary_destroy(&dictionary);
        return EXIT_FAILURE;
    }

    dictionary_drop_empty(&dictionary);

    if (dictionary_print(&dictionary)) {
        exit_status = EXIT_SUCCESS;
    }

    dictionary_destroy(&dictionary);
    return exit_status;
}