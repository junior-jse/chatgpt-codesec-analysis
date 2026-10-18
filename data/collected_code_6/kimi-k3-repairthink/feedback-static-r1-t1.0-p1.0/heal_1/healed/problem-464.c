#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#define MAX_KEY_LENGTH 256

typedef struct {
    char *key;
    int value;
} DictEntry;

typedef struct {
    DictEntry *entries;
    size_t size;
    size_t capacity;
} Dictionary;

static size_t bounded_strlen(const char *s, size_t max_len) {
    size_t len = 0;
    if (s == NULL) {
        return 0;
    }
    while (len < max_len && s[len] != '\0') {
        len++;
    }
    return len;
}

bool dict_init(Dictionary *dict, size_t capacity) {
    if (dict == NULL || capacity == 0 || capacity > SIZE_MAX / sizeof(DictEntry)) {
        return false;
    }
    dict->entries = calloc(capacity, sizeof(DictEntry));
    if (dict->entries == NULL) {
        return false;
    }
    dict->size = 0;
    dict->capacity = capacity;
    return true;
}

void dict_free(Dictionary *dict) {
    if (dict != NULL) {
        if (dict->entries != NULL) {
            for (size_t i = 0; i < dict->size; i++) {
                free(dict->entries[i].key);
                dict->entries[i].key = NULL;
            }
            free(dict->entries);
            dict->entries = NULL;
        }
        dict->size = 0;
        dict->capacity = 0;
    }
}

bool dict_add(Dictionary *dict, const char *key, int value) {
    if (dict == NULL || dict->entries == NULL || key == NULL || dict->size >= dict->capacity) {
        return false;
    }
    size_t key_len = bounded_strlen(key, MAX_KEY_LENGTH);
    if (key_len == 0 || key_len >= MAX_KEY_LENGTH) {
        return false;
    }
    char *key_copy = malloc(key_len + 1);
    if (key_copy == NULL) {
        return false;
    }
    memcpy(key_copy, key, key_len + 1);
    dict->entries[dict->size].key = key_copy;
    dict->entries[dict->size].value = value;
    dict->size++;
    return true;
}

bool all_values_same(const Dictionary *dict, bool *result) {
    if (dict == NULL || result == NULL || dict->entries == NULL || dict->size == 0) {
        return false;
    }
    *result = true;
    int first_value = dict->entries[0].value;
    for (size_t i = 1; i < dict->size; i++) {
        if (dict->entries[i].value != first_value) {
            *result = false;
            break;
        }
    }
    return true;
}

int main(void) {
    Dictionary dict;
    bool result;

    if (!dict_init(&dict, 10)) {
        fprintf(stderr, "Failed to initialize dictionary\n");
        return EXIT_FAILURE;
    }

    if (!dict_add(&dict, "a", 5) ||
        !dict_add(&dict, "b", 5) ||
        !dict_add(&dict, "c", 5)) {
        fprintf(stderr, "Failed to add entries\n");
        dict_free(&dict);
        return EXIT_FAILURE;
    }

    if (all_values_same(&dict, &result)) {
        printf("All values are %s\n", result ? "same" : "different");
    } else {
        fprintf(stderr, "Error checking values\n");
        dict_free(&dict);
        return EXIT_FAILURE;
    }

    dict_free(&dict);
    return EXIT_SUCCESS;
}