#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_KEY_LENGTH 1024

typedef struct {
    char *key;
    int value;
} Entry;

typedef struct {
    Entry *entries;
    size_t size;
    size_t capacity;
} Dict;

Dict *dict_create(size_t capacity) {
    Dict *dict = malloc(sizeof(Dict));
    if (dict == NULL) {
        return NULL;
    }
    if (capacity == 0) {
        capacity = 1;
    }
    dict->entries = calloc(capacity, sizeof(Entry));
    if (dict->entries == NULL) {
        free(dict);
        return NULL;
    }
    dict->size = 0;
    dict->capacity = capacity;
    return dict;
}

int dict_add(Dict *dict, const char *key, int value) {
    if (dict == NULL || key == NULL) {
        return 0;
    }
    if (dict->size >= dict->capacity) {
        size_t new_capacity = dict->capacity * 2;
        if (new_capacity < dict->capacity ||
            new_capacity > SIZE_MAX / sizeof(Entry)) {
            return 0;
        }
        Entry *new_entries = realloc(dict->entries, new_capacity * sizeof(Entry));
        if (new_entries == NULL) {
            return 0;
        }
        dict->entries = new_entries;
        dict->capacity = new_capacity;
    }
    size_t key_length = strnlen(key, MAX_KEY_LENGTH);
    if (key_length >= MAX_KEY_LENGTH) {
        return 0;
    }
    char *key_copy = malloc(key_length + 1);
    if (key_copy == NULL) {
        return 0;
    }
    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(key_copy, key, key_length + 1);
    dict->entries[dict->size].key = key_copy;
    dict->entries[dict->size].value = value;
    dict->size++;
    return 1;
}

void dict_free(Dict *dict) {
    if (dict != NULL) {
        if (dict->entries != NULL) {
            for (size_t i = 0; i < dict->size; i++) {
                free(dict->entries[i].key);
            }
            free(dict->entries);
        }
        free(dict);
    }
}

Dict *combine_dicts(const Dict *dict1, const Dict *dict2) {
    if (dict1 == NULL || dict2 == NULL) {
        return NULL;
    }
    if (dict1->size > SIZE_MAX - dict2->size) {
        return NULL;
    }
    Dict *result = dict_create(dict1->size + dict2->size);
    if (result == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < dict1->size; i++) {
        if (!dict_add(result, dict1->entries[i].key, dict1->entries[i].value)) {
            dict_free(result);
            return NULL;
        }
    }

    for (size_t i = 0; i < dict2->size; i++) {
        int found = 0;
        for (size_t j = 0; j < result->size; j++) {
            if (strcmp(result->entries[j].key, dict2->entries[i].key) == 0) {
                result->entries[j].value += dict2->entries[i].value;
                found = 1;
                break;
            }
        }
        if (!found) {
            if (!dict_add(result, dict2->entries[i].key, dict2->entries[i].value)) {
                dict_free(result);
                return NULL;
            }
        }
    }

    return result;
}

int main(void) {
    Dict *dict1 = dict_create(2);
    Dict *dict2 = dict_create(2);

    if (dict1 == NULL || dict2 == NULL) {
        dict_free(dict1);
        dict_free(dict2);
        return 1;
    }

    if (!dict_add(dict1, "a", 1) || !dict_add(dict1, "b", 2)) {
        dict_free(dict1);
        dict_free(dict2);
        return 1;
    }

    if (!dict_add(dict2, "b", 3) || !dict_add(dict2, "c", 4)) {
        dict_free(dict1);
        dict_free(dict2);
        return 1;
    }

    Dict *combined = combine_dicts(dict1, dict2);
    if (combined == NULL) {
        dict_free(dict1);
        dict_free(dict2);
        return 1;
    }

    for (size_t i = 0; i < combined->size; i++) {
        printf("%s: %d\n", combined->entries[i].key, combined->entries[i].value);
    }

    dict_free(dict1);
    dict_free(dict2);
    dict_free(combined);

    return 0;
}