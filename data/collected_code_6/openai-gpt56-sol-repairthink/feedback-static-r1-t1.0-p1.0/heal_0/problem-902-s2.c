#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

typedef struct {
    char *key;
    long value;
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

    for (i = 0; i < dictionary->size; ++i) {
        free(dictionary->entries[i].key);
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
    Entry *new_entries;

    if (capacity <= dictionary->capacity) {
        return 0;
    }

    if (capacity > SIZE_MAX / sizeof(*new_entries)) {
        return -1;
    }

    new_entries = realloc(dictionary->entries,
                          capacity * sizeof(*new_entries));
    if (new_entries == NULL) {
        return -1;
    }

    dictionary->entries = new_entries;
    dictionary->capacity = capacity;
    return 0;
}

static int dictionary_add(Dictionary *dictionary, const char *key, long value)
{
    size_t i;
    size_t new_capacity;
    char *key_copy;

    if (dictionary == NULL || key == NULL) {
        return -1;
    }

    for (i = 0; i < dictionary->size; ++i) {
        if (strcmp(dictionary->entries[i].key, key) == 0) {
            if ((value > 0 && dictionary->entries[i].value > LONG_MAX - value) ||
                (value < 0 && dictionary->entries[i].value < LONG_MIN - value)) {
                return -1;
            }

            dictionary->entries[i].value += value;
            return 0;
        }
    }

    if (dictionary->size == dictionary->capacity) {
        if (dictionary->capacity == 0) {
            new_capacity = 4;
        } else {
            if (dictionary->capacity > SIZE_MAX / 2) {
                return -1;
            }
            new_capacity = dictionary->capacity * 2;
        }

        if (dictionary_reserve(dictionary, new_capacity) != 0) {
            return -1;
        }
    }

    key_copy = duplicate_string(key);
    if (key_copy == NULL) {
        return -1;
    }

    dictionary->entries[dictionary->size].key = key_copy;
    dictionary->entries[dictionary->size].value = value;
    ++dictionary->size;
    return 0;
}

static int dictionary_combine(const Dictionary *first,
                              const Dictionary *second,
                              Dictionary *result)
{
    Dictionary temporary;
    size_t i;

    if (first == NULL || second == NULL || result == NULL ||
        result == first || result == second) {
        return -1;
    }

    dictionary_init(&temporary);

    for (i = 0; i < first->size; ++i) {
        if (dictionary_add(&temporary,
                           first->entries[i].key,
                           first->entries[i].value) != 0) {
            dictionary_destroy(&temporary);
            return -1;
        }
    }

    for (i = 0; i < second->size; ++i) {
        if (dictionary_add(&temporary,
                           second->entries[i].key,
                           second->entries[i].value) != 0) {
            dictionary_destroy(&temporary);
            return -1;
        }
    }

    dictionary_destroy(result);
    *result = temporary;
    return 0;
}

static void dictionary_print(const Dictionary *dictionary)
{
    size_t i;

    for (i = 0; i < dictionary->size; ++i) {
        if (printf("%s: %ld\n",
                   dictionary->entries[i].key,
                   dictionary->entries[i].value) < 0) {
            break;
        }
    }
}

int main(void)
{
    Dictionary first;
    Dictionary second;
    Dictionary combined;
    int status = EXIT_FAILURE;

    dictionary_init(&first);
    dictionary_init(&second);
    dictionary_init(&combined);

    if (dictionary_add(&first, "apple", 3) != 0 ||
        dictionary_add(&first, "banana", 5) != 0 ||
        dictionary_add(&second, "banana", 7) != 0 ||
        dictionary_add(&second, "orange", 4) != 0) {
        fprintf(stderr, "Failed to create dictionaries.\n");
        goto cleanup;
    }

    if (dictionary_combine(&first, &second, &combined) != 0) {
        fprintf(stderr, "Failed to combine dictionaries.\n");
        goto cleanup;
    }

    dictionary_print(&combined);

    if (ferror(stdout)) {
        fprintf(stderr, "Failed to write output.\n");
        goto cleanup;
    }

    status = EXIT_SUCCESS;

cleanup:
    dictionary_destroy(&combined);
    dictionary_destroy(&second);
    dictionary_destroy(&first);
    return status;
}