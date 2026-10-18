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

typedef struct {
    Dictionary *dicts;
    size_t count;
} DictList;

int dict_init(Dictionary *dict, size_t capacity) {
    if (dict == NULL) {
        return -1;
    }
    dict->entries = NULL;
    dict->size = 0;
    dict->capacity = 0;
    
    if (capacity > 0) {
        dict->entries = calloc(capacity, sizeof(DictEntry));
        if (dict->entries == NULL) {
            return -1;
        }
        dict->capacity = capacity;
    }
    return 0;
}

void dict_free(Dictionary *dict) {
    if (dict == NULL) {
        return;
    }
    if (dict->entries != NULL) {
        for (size_t i = 0; i < dict->size; i++) {
            free(dict->entries[i].key);
            free(dict->entries[i].value);
        }
        free(dict->entries);
        dict->entries = NULL;
    }
    dict->size = 0;
    dict->capacity = 0;
}

int dict_is_empty(const Dictionary *dict) {
    if (dict == NULL) {
        return 1;
    }
    return dict->size == 0;
}

int all_dicts_empty(const DictList *list) {
    if (list == NULL || list->dicts == NULL) {
        return 1;
    }
    for (size_t i = 0; i < list->count; i++) {
        if (!dict_is_empty(&list->dicts[i])) {
            return 0;
        }
    }
    return 1;
}

int dict_add(Dictionary *dict, const char *key, const char *value) {
    if (dict == NULL || key == NULL || value == NULL) {
        return -1;
    }
    
    if (dict->size >= dict->capacity) {
        size_t new_capacity = dict->capacity == 0 ? 4 : dict->capacity * 2;
        DictEntry *new_entries = realloc(dict->entries, new_capacity * sizeof(DictEntry));
        if (new_entries == NULL) {
            return -1;
        }
        dict->entries = new_entries;
        dict->capacity = new_capacity;
    }
    
    char *key_copy = malloc(strlen(key) + 1);
    if (key_copy == NULL) {
        return -1;
    }
    strcpy(key_copy, key);
    
    char *value_copy = malloc(strlen(value) + 1);
    if (value_copy == NULL) {
        free(key_copy);
        return -1;
    }
    strcpy(value_copy, value);
    
    dict->entries[dict->size].key = key_copy;
    dict->entries[dict->size].value = value_copy;
    dict->size++;
    
    return 0;
}

int main(void) {
    DictList list = {0};
    list.count = 3;
    list.dicts = calloc(list.count, sizeof(Dictionary));
    
    if (list.dicts == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }
    
    for (size_t i = 0; i < list.count; i++) {
        if (dict_init(&list.dicts[i], 0) != 0) {
            fprintf(stderr, "Failed to initialize dictionary %zu\n", i);
            for (size_t j = 0; j < i; j++) {
                dict_free(&list.dicts[j]);
            }
            free(list.dicts);
            return EXIT_FAILURE;
        }
    }
    
    printf("All empty (initial): %s\n", all_dicts_empty(&list) ? "true" : "false");
    
    if (dict_add(&list.dicts[1], "key1", "value1") != 0) {
        fprintf(stderr, "Failed to add entry\n");
        for (size_t i = 0; i < list.count; i++) {
            dict_free(&list.dicts[i]);
        }
        free(list.dicts);
        return EXIT_FAILURE;
    }
    
    printf("All empty (after add): %s\n", all_dicts_empty(&list) ? "true" : "false");
    
    for (size_t i = 0; i < list.count; i++) {
        dict_free(&list.dicts[i]);
    }
    free(list.dicts);
    
    return EXIT_SUCCESS;
}