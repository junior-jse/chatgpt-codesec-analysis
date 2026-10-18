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

void dict_init(Dictionary *dict) {
    dict->entries = NULL;
    dict->size = 0;
    dict->capacity = 0;
}

void dict_free(Dictionary *dict) {
    if (dict == NULL) {
        return;
    }
    for (size_t i = 0; i < dict->size; i++) {
        free(dict->entries[i].key);
    }
    free(dict->entries);
    dict->entries = NULL;
    dict->size = 0;
    dict->capacity = 0;
}

int dict_set(Dictionary *dict, const char *key, int value) {
    if (dict == NULL || key == NULL) {
        return -1;
    }
    
    for (size_t i = 0; i < dict->size; i++) {
        if (strcmp(dict->entries[i].key, key) == 0) {
            dict->entries[i].value = value;
            return 0;
        }
    }
    
    if (dict->size >= dict->capacity) {
        size_t new_capacity = dict->capacity == 0 ? 8 : dict->capacity * 2;
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
    
    dict->entries[dict->size].key = key_copy;
    dict->entries[dict->size].value = value;
    dict->size++;
    
    return 0;
}

int dict_merge(Dictionary *dest, const Dictionary *src) {
    if (dest == NULL || src == NULL) {
        return -1;
    }
    
    for (size_t i = 0; i < src->size; i++) {
        if (dict_set(dest, src->entries[i].key, src->entries[i].value) != 0) {
            return -1;
        }
    }
    
    return 0;
}

void dict_print(const Dictionary *dict) {
    if (dict == NULL) {
        printf("NULL dictionary\n");
        return;
    }
    printf("{");
    for (size_t i = 0; i < dict->size; i++) {
        printf("\"%s\": %d", dict->entries[i].key, dict->entries[i].value);
        if (i < dict->size - 1) {
            printf(", ");
        }
    }
    printf("}\n");
}

int main(void) {
    Dictionary dict1;
    Dictionary dict2;
    
    dict_init(&dict1);
    dict_init(&dict2);
    
    if (dict_set(&dict1, "apple", 5) != 0) {
        fprintf(stderr, "Failed to set key in dict1\n");
        dict_free(&dict1);
        dict_free(&dict2);
        return EXIT_FAILURE;
    }
    if (dict_set(&dict1, "banana", 3) != 0) {
        fprintf(stderr, "Failed to set key in dict1\n");
        dict_free(&dict1);
        dict_free(&dict2);
        return EXIT_FAILURE;
    }
    
    if (dict_set(&dict2, "banana", 7) != 0) {
        fprintf(stderr, "Failed to set key in dict2\n");
        dict_free(&dict1);
        dict_free(&dict2);
        return EXIT_FAILURE;
    }
    if (dict_set(&dict2, "cherry", 10) != 0) {
        fprintf(stderr, "Failed to set key in dict2\n");
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