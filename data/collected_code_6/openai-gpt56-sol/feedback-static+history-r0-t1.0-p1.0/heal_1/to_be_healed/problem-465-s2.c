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

static void free_dictionary(Dictionary *dictionary)
{
    size_t i;

    if (dictionary == NULL) {
        return;
    }

    for (i = 0U; i < dictionary->size; ++i) {
        free(dictionary->items[i].key);
        free(dictionary->items[i].value);
    }

    free(dictionary->items);
    dictionary->items = NULL;
    dictionary->size = 0U;
}

static int drop_empty_items(Dictionary *dictionary)
{
    size_t read_index;
    size_t write_index;

    if (dictionary == NULL ||
        (dictionary->size > 0U && dictionary->items == NULL)) {
        return -1;
    }

    write_index = 0U;

    for (read_index = 0U; read_index < dictionary->size; ++read_index) {
        DictionaryItem *item = &dictionary->items[read_index];

        if (item->key == NULL || item->key[0] == '\0' ||
            item->value == NULL || item->value[0] == '\0') {
            free(item->key);
            free(item->value);
            item->key = NULL;
            item->value = NULL;
        } else {
            if (write_index != read_index) {
                dictionary->items[write_index] = *item;
                item->key = NULL;
                item->value = NULL;
            }
            ++write_index;
        }
    }

    dictionary->size = write_index;

    if (write_index == 0U) {
        free(dictionary->items);
        dictionary->items = NULL;
    } else {
        DictionaryItem *resized_items =
            realloc(dictionary->items, write_index * sizeof(*resized_items));

        if (resized_items != NULL) {
            dictionary->items = resized_items;
        }
    }

    return 0;
}

static int initialize_dictionary(Dictionary *dictionary)
{
    static const char *const keys[] = {
        "name", "empty_value", "", "language"
    };
    static const char *const values[] = {
        "Alice", "", "ignored", "C"
    };
    const size_t count = sizeof(keys) / sizeof(keys[0]);
    size_t i;

    if (dictionary == NULL) {
        return -1;
    }

    dictionary->items = calloc(count, sizeof(*dictionary->items));
    dictionary->size = 0U;

    if (dictionary->items == NULL) {
        return -1;
    }

    for (i = 0U; i < count; ++i) {
        dictionary->items[i].key = duplicate_string(keys[i]);
        dictionary->items[i].value = duplicate_string(values[i]);

        if (dictionary->items[i].key == NULL ||
            dictionary->items[i].value == NULL) {
            dictionary->size = i + 1U;
            free_dictionary(dictionary);
            return -1;
        }

        dictionary->size = i + 1U;
    }

    return 0;
}

int main(void)
{
    Dictionary dictionary = {NULL, 0U};
    size_t i;

    if (initialize_dictionary(&dictionary) != 0) {
        fputs("Failed to initialize dictionary.\n", stderr);
        return EXIT_FAILURE;
    }

    if (drop_empty_items(&dictionary) != 0) {
        fputs("Failed to process dictionary.\n", stderr);
        free_dictionary(&dictionary);
        return EXIT_FAILURE;
    }

    for (i = 0U; i < dictionary.size; ++i) {
        if (printf("%s: %s\n",
                   dictionary.items[i].key,
                   dictionary.items[i].value) < 0) {
            fputs("Failed to write output.\n", stderr);
            free_dictionary(&dictionary);
            return EXIT_FAILURE;
        }
    }

    free_dictionary(&dictionary);
    return EXIT_SUCCESS;
}