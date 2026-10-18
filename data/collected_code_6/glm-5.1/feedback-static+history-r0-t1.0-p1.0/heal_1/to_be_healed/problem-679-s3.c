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
    Dictionary *dict = malloc(sizeof(Dictionary));
    if (!dict) return NULL;
    dict->entries = malloc(initial_capacity * sizeof(DictionaryEntry));
    if (!dict->entries) {
        free(dict);
        return NULL;
    }
    dict->size = 0;
    dict->capacity = initial_capacity;
    return dict;
}

void dictionary_destroy(Dictionary *dict) {
    if (!dict) return;
    if (dict->entries) {
        for (size_t i = 0; i < dict->size; ++i) {
            free(dict->entries[i].key);
        }
        free(dict->entries);
    }
    free(dict);
}

int dictionary_insert(Dictionary *dict, const char *key, int value) {
    if (!dict || !key) return -1;
    if (dict->size == dict->capacity) {
        size_t new_capacity = dict->capacity * 2;
        DictionaryEntry *new_entries = realloc(dict->entries, new_capacity * sizeof(DictionaryEntry));
        if (!new_entries) return -1;
        dict->entries = new_entries;
        dict->capacity = new_capacity;
    }
    char *new_key = strdup(key);
    if (!new_key) return -1;
    dict->entries[dict->size].key = new_key;
    dict->entries[dict->size].value = value;
    dict->size++;
    return 0;
}

int *dictionary_get_by_index(Dictionary *dict, size_t index) {
    if (!dict) return NULL;
    if (index >= dict->size) return NULL;
    return &dict->entries[index].value;
}

int main(void) {
    Dictionary *dict = dictionary_create(4);
    if (!dict) {
        fprintf(stderr, "Failed to create dictionary\n");
        return 1;
    }

    dictionary_insert(dict, "alpha", 10);
    dictionary_insert(dict, "beta", 20);
    dictionary_insert(dict, "gamma", 30);

    size_t target_index = 1;
    /* Possible weaknesses found:
     *  Variable 'value' can be declared as pointer to const [constVariablePointer]
     */
    int *value = dictionary_get_by_index(dict, target_index);
    if (value) {
        printf("Value at index %zu: %d\n", target_index, *value);
    } else {
        printf("Invalid index or null dictionary\n");
    }

    size_t invalid_index = 5;
    /* Possible weaknesses found:
     *  Variable 'invalid_value' can be declared as pointer to const [constVariablePointer]
     */
    int *invalid_value = dictionary_get_by_index(dict, invalid_index);
    if (!invalid_value) {
        printf("Correctly handled invalid index %zu\n", invalid_index);
    }

    dictionary_destroy(dict);
    return 0;
}