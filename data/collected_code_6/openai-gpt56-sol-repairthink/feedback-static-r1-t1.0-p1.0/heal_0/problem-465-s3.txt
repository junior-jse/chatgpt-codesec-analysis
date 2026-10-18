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
    size_t count;
} Dictionary;

static void free_dictionary(Dictionary *dictionary)
{
    size_t i;

    if (dictionary == NULL) {
        return;
    }

    for (i = 0; i < dictionary->count; ++i) {
        free(dictionary->items[i].key);
        free(dictionary->items[i].value);
    }

    free(dictionary->items);
    dictionary->items = NULL;
    dictionary->count = 0;
}

static bool is_empty_item(const DictionaryItem *item)
{
    return item == NULL ||
           item->key == NULL ||
           item->key[0] == '\0' ||
           item->value == NULL ||
           item->value[0] == '\0';
}

static void drop_empty_items(Dictionary *dictionary)
{
    size_t read_index;
    size_t write_index = 0;

    if (dictionary == NULL || dictionary->items == NULL) {
        return;
    }

    for (read_index = 0; read_index < dictionary->count; ++read_index) {
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

    dictionary->count = write_index;

    if (write_index == 0) {
        free(dictionary->items);
        dictionary->items = NULL;
    } else {
        DictionaryItem *resized_items =
            realloc(dictionary->items, write_index * sizeof(*resized_items));

        if (resized_items != NULL) {
            dictionary->items = resized_items;
        }
    }
}

static char *copy_string(const char *source)
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

static bool initialize_dictionary(
    Dictionary *dictionary,
    const char *const keys[],
    const char *const values[],
    size_t count)
{
    size_t i;

    if (dictionary == NULL || (count > 0 && (keys == NULL || values == NULL))) {
        return false;
    }

    dictionary->items = NULL;
    dictionary->count = 0;

    if (count == 0) {
        return true;
    }

    if (count > SIZE_MAX / sizeof(*dictionary->items)) {
        return false;
    }

    dictionary->items = calloc(count, sizeof(*dictionary->items));
    if (dictionary->items == NULL) {
        return false;
    }

    dictionary->count = count;

    for (i = 0; i < count; ++i) {
        if (keys[i] != NULL) {
            dictionary->items[i].key = copy_string(keys[i]);
            if (dictionary->items[i].key == NULL) {
                free_dictionary(dictionary);
                return false;
            }
        }

        if (values[i] != NULL) {
            dictionary->items[i].value = copy_string(values[i]);
            if (dictionary->items[i].value == NULL) {
                free_dictionary(dictionary);
                return false;
            }
        }
    }

    return true;
}

int main(void)
{
    const char *keys[] = {"name", "email", "", "country", "phone"};
    const char *values[] = {"Ada", "", "unused", "UK", NULL};
    Dictionary dictionary;
    size_t i;

    if (!initialize_dictionary(
            &dictionary,
            keys,
            values,
            sizeof(keys) / sizeof(keys[0]))) {
        fputs("Failed to initialize dictionary.\n", stderr);
        return EXIT_FAILURE;
    }

    drop_empty_items(&dictionary);

    for (i = 0; i < dictionary.count; ++i) {
        if (printf("%s: %s\n",
                   dictionary.items[i].key,
                   dictionary.items[i].value) < 0) {
            free_dictionary(&dictionary);
            return EXIT_FAILURE;
        }
    }

    free_dictionary(&dictionary);
    return EXIT_SUCCESS;
}