#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *key;
    char *value;
} DictEntry;

typedef struct {
    DictEntry *entries;
    size_t size;
    size_t capacity;
} Dictionary;

Dictionary *dict_create(size_t initial_capacity) {
    Dictionary *dict = malloc(sizeof(Dictionary));
    if (!dict) return NULL;
    dict->entries = malloc(initial_capacity * sizeof(DictEntry));
    if (!dict->entries) {
        free(dict);
        return NULL;
    }
    dict->size = 0;
    dict->capacity = initial_capacity;
    return dict;
}

int dict_insert(Dictionary *dict, const char *key, const char *value) {
    if (!dict || !key || !value) return 0;
    if (dict->size >= dict->capacity) {
        size_t new_capacity = dict->capacity * 2;
        DictEntry *new_entries = realloc(dict->entries, new_capacity * sizeof(DictEntry));
        if (!new_entries) return 0;
        dict->entries = new_entries;
        dict->capacity = new_capacity;
    }
    for (size_t i = 0; i < dict->size; i++) {
        if (strcmp(dict->entries[i].key, key) == 0) {
            char *new_value = strdup(value);
            if (!new_value) return 0;
            free(dict->entries[i].value);
            dict->entries[i].value = new_value;
            return 1;
        }
    }
    char *new_key = strdup(key);
    if (!new_key) return 0;
    char *new_value = strdup(value);
    if (!new_value) {
        free(new_key);
        return 0;
    }
    dict->entries[dict->size].key = new_key;
    dict->entries[dict->size].value = new_value;
    dict->size++;
    return 1;
}

void dict_free(Dictionary *dict) {
    if (!dict) return;
    for (size_t i = 0; i < dict->size; i++) {
        free(dict->entries[i].key);
        free(dict->entries[i].value);
    }
    free(dict->entries);
    free(dict);
}

Dictionary *dict_merge(const Dictionary *d1, const Dictionary *d2) {
    if (!d1 && !d2) return NULL;
    size_t capacity = (d1 ? d1->size : 0) + (d2 ? d2->size : 0);
    if (capacity == 0) capacity = 1;
    
    Dictionary *merged = dict_create(capacity);
    if (!merged) return NULL;
    
    if (d1) {
        for (size_t i = 0; i < d1->size; i++) {
            if (!dict_insert(merged, d1->entries[i].key, d1->entries[i].value)) {
                dict_free(merged);
                return NULL;
            }
        }
    }
    
    if (d2) {
        for (size_t i = 0; i < d2->size; i++) {
            if (!dict_insert(merged, d2->entries[i].key, d2->entries[i].value)) {
                dict_free(merged);
                return NULL;
            }
        }
    }
    
    return merged;
}

void dict_print(const Dictionary *dict) {
    if (!dict) return;
    for (size_t i = 0; i < dict->size; i++) {
        printf("%s: %s\n", dict->entries[i].key, dict->entries[i].value);
    }
}

int main(void) {
    Dictionary *d1 = dict_create(4);
    if (!d1) return EXIT_FAILURE;
    
    Dictionary *d2 = dict_create(4);
    if (!d2) {
        dict_free(d1);
        return EXIT_FAILURE;
    }
    
    if (!dict_insert(d1, "a", "1") || !dict_insert(d1, "b", "2")) {
        dict_free(d1);
        dict_free(d2);
        return EXIT_FAILURE;
    }
    
    if (!dict_insert(d2, "b", "3") || !dict_insert(d2, "c", "4")) {
        dict_free(d1);
        dict_free(d2);
        return EXIT_FAILURE;
    }
    
    Dictionary *merged = dict_merge(d1, d2);
    if (!merged) {
        dict_free(d1);
        dict_free(d2);
        return EXIT_FAILURE;
    }
    
    dict_print(merged);
    
    dict_free(d1);
    dict_free(d2);
    dict_free(merged);
    
    return EXIT_SUCCESS;
}