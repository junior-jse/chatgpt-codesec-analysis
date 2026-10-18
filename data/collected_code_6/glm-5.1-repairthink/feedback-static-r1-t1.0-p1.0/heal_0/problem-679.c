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

Dictionary *dictionary_create(size_t initial_capacity) {
    Dictionary *dict = (Dictionary *)malloc(sizeof(Dictionary));
    if (!dict) {
        return NULL;
    }
    dict->entries = (DictionaryEntry *)malloc(initial_capacity * sizeof(DictionaryEntry));
    if (!dict->entries) {
        free(dict);
        return NULL;
    }
    dict->size = 0;
    dict->capacity = initial_capacity;
    return dict;
}

void dictionary_destroy(Dictionary *dict) {
    if (dict) {
        for (size_t i = 0; i < dict->size; i++) {
            free(dict->entries[i].key);
        }
        free(dict->entries);
        free(dict);
    }
}

int dictionary_insert(Dictionary *dict, const char *key, int value) {
    if (!dict || !key) {
        return -1;
    }
    if (dict->size == dict->capacity) {
        size_t new_capacity = dict->capacity * 2;
        DictionaryEntry *new_entries = (DictionaryEntry *)realloc(dict->entries, new_capacity * sizeof(DictionaryEntry));
        if (!new_entries) {
            return -1;
        }
        dict->entries = new_entries;
        dict->capacity = new_capacity;
    }
    char *new_key = (char *)malloc(strlen(key) + 1);
    if (!new_key) {
        return -1;
    }
    strcpy(new_key, key);
    dict->entries[dict->size].key = new_key;
    dict->entries[dict->size].value = value;
    dict->size++;
    return 0;
}

const char *dictionary_get_key_by_index(const Dictionary *dict, size_t index) {
    if (!dict) {
        return NULL;
    }
    if (index >= dict->size) {
        return NULL;
    }
    return dict->entries[index].key;
}

int dictionary_get_value_by_index(const Dictionary *dict, size_t index, int *out_value) {
    if (!dict || !out_value) {
        return -1;
    }
    if (index >= dict->size) {
        return -1;
    }
    *out_value = dict->entries[index].value;
    return 0;
}

int main(void) {
    Dictionary *dict = dictionary_create(4);
    if (!dict) {
        return EXIT_FAILURE;
    }

    if (dictionary_insert(dict, "apple", 10) != 0) {
        dictionary_destroy(dict);
        return EXIT_FAILURE;
    }
    if (dictionary_insert(dict, "banana", 20) != 0) {
        dictionary_destroy(dict);
        return EXIT_FAILURE;
    }
    if (dictionary_insert(dict, "cherry", 30) != 0) {
        dictionary_destroy(dict);
        return EXIT_FAILURE;
    }

    size_t query_index = 1;
    const char *key = dictionary_get_key_by_index(dict, query_index);
    int value;
    int status = dictionary_get_value_by_index(dict, query_index, &value);

    if (key && status == 0) {
        printf("Index %zu: Key = %s, Value = %d\n", query_index, key, value);
    } else {
        printf("Failed to access index %zu\n", query_index);
    }

    key = dictionary_get_key_by_index(dict, 5);
    status = dictionary_get_value_by_index(dict, 5, &value);
    if (!key || status != 0) {
        printf("Properly handled out-of-bounds access for index 5\n");
    }

    dictionary_destroy(dict);
    dict = NULL;

    return EXIT_SUCCESS;
}