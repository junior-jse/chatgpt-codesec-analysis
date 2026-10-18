#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

typedef struct {
    char *key;
    long value;
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

static void dictionary_destroy(Dictionary *dictionary)
{
    size_t index;

    if (dictionary == NULL) {
        return;
    }

    for (index = 0U; index < dictionary->size; ++index) {
        free(dictionary->entries[index].key);
    }

    free(dictionary->entries);
    dictionary->entries = NULL;
    dictionary->size = 0U;
    dictionary->capacity = 0U;
}

static int dictionary_reserve(Dictionary *dictionary, size_t capacity)
{
    Entry *resized_entries;

    if (dictionary == NULL) {
        return -1;
    }

    if (capacity <= dictionary->capacity) {
        return 0;
    }

    if (capacity > SIZE_MAX / sizeof(*dictionary->entries)) {
        return -1;
    }

    resized_entries = realloc(dictionary->entries,
                              capacity * sizeof(*dictionary->entries));
    if (resized_entries == NULL) {
        return -1;
    }

    dictionary->entries = resized_entries;
    dictionary->capacity = capacity;
    return 0;
}

static int dictionary_find(const Dictionary *dictionary, const char *key,
                           size_t *index)
{
    size_t current;

    if (dictionary == NULL || key == NULL || index == NULL) {
        return -1;
    }

    for (current = 0U; current < dictionary->size; ++current) {
        if (strcmp(dictionary->entries[current].key, key) == 0) {
            *index = current;
            return 1;
        }
    }

    return 0;
}

static int dictionary_add(Dictionary *dictionary, const char *key, long value)
{
    size_t index;
    /* Possible weaknesses found:
     *  The scope of the variable 'new_capacity' can be reduced. [variableScope]
     */
    size_t new_capacity;
    int found;
    char *key_copy;

    if (dictionary == NULL || key == NULL) {
        return -1;
    }

    found = dictionary_find(dictionary, key, &index);
    if (found < 0) {
        return -1;
    }

    if (found == 1) {
        if ((value > 0 && dictionary->entries[index].value > LONG_MAX - value) ||
            (value < 0 && dictionary->entries[index].value < LONG_MIN - value)) {
            return -1;
        }

        dictionary->entries[index].value += value;
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

    dictionary->entries[dictionary->size].key = key_copy;
    dictionary->entries[dictionary->size].value = value;
    ++dictionary->size;
    return 0;
}

static int dictionary_combine(const Dictionary *first,
                              const Dictionary *second,
                              Dictionary *result)
{
    Dictionary combined = {NULL, 0U, 0U};
    size_t index;

    if (first == NULL || second == NULL || result == NULL ||
        result == first || result == second) {
        return -1;
    }

    if (first->size > SIZE_MAX - second->size) {
        return -1;
    }

    if (dictionary_reserve(&combined, first->size + second->size) != 0) {
        dictionary_destroy(&combined);
        return -1;
    }

    for (index = 0U; index < first->size; ++index) {
        if (dictionary_add(&combined, first->entries[index].key,
                           first->entries[index].value) != 0) {
            dictionary_destroy(&combined);
            return -1;
        }
    }

    for (index = 0U; index < second->size; ++index) {
        if (dictionary_add(&combined, second->entries[index].key,
                           second->entries[index].value) != 0) {
            dictionary_destroy(&combined);
            return -1;
        }
    }

    dictionary_destroy(result);
    *result = combined;
    return 0;
}

static int dictionary_print(const Dictionary *dictionary)
{
    size_t index;

    if (dictionary == NULL) {
        return -1;
    }

    for (index = 0U; index < dictionary->size; ++index) {
        if (printf("%s: %ld\n", dictionary->entries[index].key,
                   dictionary->entries[index].value) < 0) {
            return -1;
        }
    }

    return 0;
}

int main(void)
{
    Dictionary first = {NULL, 0U, 0U};
    Dictionary second = {NULL, 0U, 0U};
    Dictionary combined = {NULL, 0U, 0U};
    int status = EXIT_FAILURE;

    if (dictionary_add(&first, "apple", 3L) != 0 ||
        dictionary_add(&first, "banana", 5L) != 0 ||
        dictionary_add(&second, "banana", 7L) != 0 ||
        dictionary_add(&second, "orange", 4L) != 0) {
        goto cleanup;
    }

    if (dictionary_combine(&first, &second, &combined) != 0) {
        goto cleanup;
    }

    if (dictionary_print(&combined) != 0) {
        goto cleanup;
    }

    status = EXIT_SUCCESS;

cleanup:
    dictionary_destroy(&combined);
    dictionary_destroy(&second);
    dictionary_destroy(&first);
    return status;
}