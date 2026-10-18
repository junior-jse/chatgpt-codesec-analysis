#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *key;
    int value;
} DictionaryEntry;

typedef struct {
    DictionaryEntry *entries;
    size_t size;
    size_t capacity;
} Dictionary;

Dictionary *dictionary_create(size_t capacity) {
    Dictionary *dict = malloc(sizeof(Dictionary));
    if (!dict) return NULL;
    dict->entries = malloc(capacity * sizeof(DictionaryEntry));
    if (!dict->entries) {
        free(dict);
        return NULL;
    }
    dict->size = 0;
    dict->capacity = capacity;
    return dict;
}

int dictionary_insert(Dictionary *dict, const char *key, int value) {
    if (!dict || !key || dict->size >= dict->capacity) return -1;
    dict->entries[dict->size].key = malloc(strlen(key) + 1);
    if (!dict->entries[dict->size].key) return -1;
    strcpy(dict->entries[dict->size].key, key);
    dict->entries[dict->size].value = value;
    dict->size++;
    return 0;
}

int dictionary_get_key_by_index(Dictionary *dict, size_t index, char **key) {
    if (!dict || !key || index >= dict->size) return -1;
    *key = dict->entries[index].key;
    return 0;
}

void dictionary_destroy(Dictionary *dict) {
    if (!dict) return;
    for (size_t i = 0; i < dict->size; i++) {
        free(dict->entries[i].key);
    }
    free(dict->entries);
    free(dict);
}

int main(void) {
    Dictionary *dict = dictionary_create(3);
    if (!dict) return EXIT_FAILURE;

    if (dictionary_insert(dict, "alpha", 1) != 0) {
        dictionary_destroy(dict);
        return EXIT_FAILURE;
    }
    if (dictionary_insert(dict, "beta", 2) != 0) {
        dictionary_destroy(dict);
        return EXIT_FAILURE;
    }
    if (dictionary_insert(dict, "gamma", 3) != 0) {
        dictionary_destroy(dict);
        return EXIT_FAILURE;
    }

    char *key = NULL;
    size_t index_to_access = 1;

    if (dictionary_get_key_by_index(dict, index_to_access, &key) == 0) {
        printf("Key at index %zu: %s\n", index_to_access, key);
    } else {
        printf("Failed to access index %zu\n", index_to_access);
    }

    index_to_access = 5;
    if (dictionary_get_key_by_index(dict, index_to_access, &key) == 0) {
        printf("Key at index %zu: %s\n", index_to_access, key);
    } else {
        printf("Failed to access index %zu\n", index_to_access);
    }

    dictionary_destroy(dict);
    dict = NULL;

    return EXIT_SUCCESS;
}