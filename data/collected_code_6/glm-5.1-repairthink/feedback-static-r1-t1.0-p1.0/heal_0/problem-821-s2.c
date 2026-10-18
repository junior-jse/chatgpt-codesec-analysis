#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *key;
    char *value;
} DictionaryEntry;

typedef struct {
    DictionaryEntry *entries;
    size_t size;
    size_t capacity;
} Dictionary;

Dictionary *dictionary_create(size_t initial_capacity) {
    Dictionary *dict = (Dictionary *)malloc(sizeof(Dictionary));
    if (dict == NULL) return NULL;

    dict->entries = (DictionaryEntry *)malloc(initial_capacity * sizeof(DictionaryEntry));
    if (dict->entries == NULL) {
        free(dict);
        return NULL;
    }

    dict->size = 0;
    dict->capacity = initial_capacity;
    return dict;
}

int dictionary_insert(Dictionary *dict, const char *key, const char *value) {
    if (dict == NULL || key == NULL || value == NULL) return 0;

    if (dict->size >= dict->capacity) {
        size_t new_capacity = dict->capacity * 2;
        DictionaryEntry *new_entries = (DictionaryEntry *)realloc(dict->entries, new_capacity * sizeof(DictionaryEntry));
        if (new_entries == NULL) return 0;
        dict->entries = new_entries;
        dict->capacity = new_capacity;
    }

    char *key_copy = strdup(key);
    if (key_copy == NULL) return 0;

    char *value_copy = strdup(value);
    if (value_copy == NULL) {
        free(key_copy);
        return 0;
    }

    dict->entries[dict->size].key = key_copy;
    dict->entries[dict->size].value = value_copy;
    dict->size++;
    return 1;
}

void dictionary_destroy(Dictionary *dict) {
    if (dict == NULL) return;
    for (size_t i = 0; i < dict->size; i++) {
        free(dict->entries[i].key);
        free(dict->entries[i].value);
    }
    free(dict->entries);
    free(dict);
}

Dictionary *dictionary_merge(const Dictionary *dict1, const Dictionary *dict2) {
    if (dict1 == NULL && dict2 == NULL) return NULL;

    size_t size1 = (dict1 != NULL) ? dict1->size : 0;
    size_t size2 = (dict2 != NULL) ? dict2->size : 0;
    size_t total_capacity = size1 + size2;

    if (total_capacity == 0) total_capacity = 1;

    Dictionary *merged = dictionary_create(total_capacity);
    if (merged == NULL) return NULL;

    if (dict1 != NULL) {
        for (size_t i = 0; i < dict1->size; i++) {
            if (!dictionary_insert(merged, dict1->entries[i].key, dict1->entries[i].value)) {
                dictionary_destroy(merged);
                return NULL;
            }
        }
    }

    if (dict2 != NULL) {
        for (size_t i = 0; i < dict2->size; i++) {
            if (!dictionary_insert(merged, dict2->entries[i].key, dict2->entries[i].value)) {
                dictionary_destroy(merged);
                return NULL;
            }
        }
    }

    return merged;
}

int main(void) {
    Dictionary *dict1 = dictionary_create(4);
    Dictionary *dict2 = dictionary_create(4);

    if (dict1 == NULL || dict2 == NULL) {
        dictionary_destroy(dict1);
        dictionary_destroy(dict2);
        return EXIT_FAILURE;
    }

    if (!dictionary_insert(dict1, "key1", "value1") ||
        !dictionary_insert(dict1, "key2", "value2") ||
        !dictionary_insert(dict2, "key3", "value3") ||
        !dictionary_insert(dict2, "key4", "value4")) {
        dictionary_destroy(dict1);
        dictionary_destroy(dict2);
        return EXIT_FAILURE;
    }

    Dictionary *merged = dictionary_merge(dict1, dict2);
    if (merged == NULL) {
        dictionary_destroy(dict1);
        dictionary_destroy(dict2);
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < merged->size; i++) {
        printf("%s: %s\n", merged->entries[i].key, merged->entries[i].value);
    }

    dictionary_destroy(dict1);
    dictionary_destroy(dict2);
    dictionary_destroy(merged);

    return EXIT_SUCCESS;
}