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

int dict_init(Dictionary *dict, size_t capacity) {
    if (dict == NULL || capacity == 0) {
        return -1;
    }
    dict->entries = malloc(capacity * sizeof(DictEntry));
    if (dict->entries == NULL) {
        return -1;
    }
    dict->size = 0;
    dict->capacity = capacity;
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

int dict_add(Dictionary *dict, const char *key, const char *value) {
    if (dict == NULL || key == NULL || value == NULL) {
        return -1;
    }
    if (dict->size >= dict->capacity) {
        size_t new_capacity = dict->capacity * 2;
        DictEntry *new_entries = realloc(dict->entries, new_capacity * sizeof(DictEntry));
        if (new_entries == NULL) {
            return -1;
        }
        dict->entries = new_entries;
        dict->capacity = new_capacity;
    }
    
    size_t key_len = strnlen(key, 1024);
    if (key_len == 1024 && key[key_len] != '\0') {
        return -1;
    }
    
    dict->entries[dict->size].key = malloc(key_len + 1);
    if (dict->entries[dict->size].key == NULL) {
        return -1;
    }
    snprintf(dict->entries[dict->size].key, key_len + 1, "%s", key);
    
    size_t value_len = strnlen(value, 1024);
    if (value_len == 1024 && value[value_len] != '\0') {
        free(dict->entries[dict->size].key);
        return -1;
    }
    
    dict->entries[dict->size].value = malloc(value_len + 1);
    if (dict->entries[dict->size].value == NULL) {
        free(dict->entries[dict->size].key);
        return -1;
    }
    snprintf(dict->entries[dict->size].value, value_len + 1, "%s", value);
    
    dict->size++;
    return 0;
}

char *dict_get(const Dictionary *dict, const char *key) {
    if (dict == NULL || key == NULL) {
        return NULL;
    }
    for (size_t i = 0; i < dict->size; i++) {
        if (strcmp(dict->entries[i].key, key) == 0) {
            return dict->entries[i].value;
        }
    }
    return NULL;
}

int dict_merge(Dictionary *dest, const Dictionary *src) {
    if (dest == NULL || src == NULL) {
        return -1;
    }
    for (size_t i = 0; i < src->size; i++) {
        if (dict_get(dest, src->entries[i].key) == NULL) {
            if (dict_add(dest, src->entries[i].key, src->entries[i].value) != 0) {
                return -1;
            }
        }
    }
    return 0;
}

void dict_print(const Dictionary *dict) {
    if (dict == NULL) {
        return;
    }
    printf("{");
    for (size_t i = 0; i < dict->size; i++) {
        printf("'%s': '%s'", dict->entries[i].key, dict->entries[i].value);
        if (i < dict->size - 1) {
            printf(", ");
        }
    }
    printf("}\n");
}

int main(void) {
    Dictionary dict1, dict2;
    
    if (dict_init(&dict1, 4) != 0) {
        fprintf(stderr, "Failed to initialize dict1\n");
        return EXIT_FAILURE;
    }
    
    if (dict_init(&dict2, 4) != 0) {
        fprintf(stderr, "Failed to initialize dict2\n");
        dict_free(&dict1);
        return EXIT_FAILURE;
    }
    
    if (dict_add(&dict1, "a", "1") != 0 ||
        dict_add(&dict1, "b", "2") != 0 ||
        dict_add(&dict1, "c", "3") != 0) {
        fprintf(stderr, "Failed to add entries to dict1\n");
        dict_free(&dict1);
        dict_free(&dict2);
        return EXIT_FAILURE;
    }
    
    if (dict_add(&dict2, "d", "4") != 0 ||
        dict_add(&dict2, "e", "5") != 0 ||
        dict_add(&dict2, "b", "override") != 0) {
        fprintf(stderr, "Failed to add entries to dict2\n");
        dict_free(&dict1);
        dict_free(&dict2);
        return EXIT_FAILURE;
    }
    
    printf("Dict1: ");
    dict_print(&dict1);
    
    printf("Dict2: ");
    dict_print(&dict2);
    
    if (dict_merge(&dict1, &dict2) != 0) {
        fprintf(stderr, "Failed to merge dictionaries\n");
        dict_free(&dict1);
        dict_free(&dict2);
        return EXIT_FAILURE;
    }
    
    printf("Merged: ");
    dict_print(&dict1);
    
    dict_free(&dict1);
    dict_free(&dict2);
    
    return EXIT_SUCCESS;
}