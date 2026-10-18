#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *key;
    int value;
} DictEntry;

typedef struct {
    DictEntry *entries;
    size_t size;
    size_t capacity;
} Dictionary;

int dict_init(Dictionary *dict, size_t initial_capacity) {
    if (dict == NULL || initial_capacity == 0) {
        return -1;
    }
    dict->entries = malloc(initial_capacity * sizeof(DictEntry));
    if (dict->entries == NULL) {
        return -1;
    }
    dict->size = 0;
    dict->capacity = initial_capacity;
    return 0;
}

void dict_free(Dictionary *dict) {
    size_t i;
    if (dict == NULL) {
        return;
    }
    if (dict->entries != NULL) {
        for (i = 0; i < dict->size; i++) {
            free(dict->entries[i].key);
            dict->entries[i].key = NULL;
        }
        free(dict->entries);
        dict->entries = NULL;
    }
    dict->size = 0;
    dict->capacity = 0;
}

int dict_add(Dictionary *dict, const char *key, int value) {
    DictEntry *new_entries;
    size_t new_capacity;
    size_t key_len;
    
    if (dict == NULL || key == NULL || dict->entries == NULL) {
        return -1;
    }
    
    if (dict->size >= dict->capacity) {
        new_capacity = dict->capacity * 2;
        if (new_capacity < dict->capacity) {
            return -1;
        }
        new_entries = realloc(dict->entries, new_capacity * sizeof(DictEntry));
        if (new_entries == NULL) {
            return -1;
        }
        dict->entries = new_entries;
        dict->capacity = new_capacity;
    }
    
    key_len = strlen(key);
    dict->entries[dict->size].key = malloc(key_len + 1);
    if (dict->entries[dict->size].key == NULL) {
        return -1;
    }
    
    strcpy(dict->entries[dict->size].key, key);
    dict->entries[dict->size].value = value;
    dict->size++;
    return 0;
}

int dict_get_by_index(const Dictionary *dict, size_t index, const char **key, int *value) {
    if (dict == NULL || key == NULL || value == NULL) {
        return -1;
    }
    if (dict->entries == NULL || index >= dict->size) {
        return -1;
    }
    *key = dict->entries[index].key;
    *value = dict->entries[index].value;
    return 0;
}

int main(void) {
    Dictionary dict;
    const char *key;
    int value;
    size_t i;
    int status;
    
    status = dict_init(&dict, 4);
    if (status != 0) {
        fprintf(stderr, "Failed to initialize dictionary\n");
        return EXIT_FAILURE;
    }
    
    if (dict_add(&dict, "apple", 10) != 0 ||
        dict_add(&dict, "banana", 20) != 0 ||
        dict_add(&dict, "cherry", 30) != 0) {
        fprintf(stderr, "Failed to add entries\n");
        dict_free(&dict);
        return EXIT_FAILURE;
    }
    
    for (i = 0; i < dict.size; i++) {
        if (dict_get_by_index(&dict, i, &key, &value) == 0) {
            printf("Index %zu: %s = %d\n", i, key, value);
        } else {
            fprintf(stderr, "Failed to access index %zu\n", i);
        }
    }
    
    dict_free(&dict);
    return EXIT_SUCCESS;
}