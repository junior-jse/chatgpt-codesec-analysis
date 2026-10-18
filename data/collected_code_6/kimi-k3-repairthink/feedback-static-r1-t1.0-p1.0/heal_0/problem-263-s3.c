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

void dict_init(Dictionary *dict, size_t initial_capacity) {
    dict->entries = malloc(initial_capacity * sizeof(DictEntry));
    if (dict->entries == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    dict->size = 0;
    dict->capacity = initial_capacity;
}

void dict_free(Dictionary *dict) {
    size_t i;
    for (i = 0; i < dict->size; i++) {
        free(dict->entries[i].key);
        free(dict->entries[i].value);
    }
    free(dict->entries);
    dict->entries = NULL;
    dict->size = 0;
    dict->capacity = 0;
}

char *string_duplicate(const char *str) {
    size_t len;
    char *copy;
    
    if (str == NULL) {
        return NULL;
    }
    
    len = strlen(str) + 1;
    copy = malloc(len);
    if (copy == NULL) {
        return NULL;
    }
    
    memcpy(copy, str, len);
    return copy;
}

int dict_resize(Dictionary *dict) {
    size_t new_capacity;
    DictEntry *new_entries;
    
    new_capacity = dict->capacity * 2;
    new_entries = realloc(dict->entries, new_capacity * sizeof(DictEntry));
    
    if (new_entries == NULL) {
        return -1;
    }
    
    dict->entries = new_entries;
    dict->capacity = new_capacity;
    return 0;
}

int dict_find_index(const Dictionary *dict, const char *key) {
    size_t i;
    
    for (i = 0; i < dict->size; i++) {
        if (strcmp(dict->entries[i].key, key) == 0) {
            return (int)i;
        }
    }
    
    return -1;
}

int dict_set(Dictionary *dict, const char *key, const char *value) {
    int index;
    char *new_key;
    char *new_value;
    
    if (dict == NULL || key == NULL || value == NULL) {
        return -1;
    }
    
    index = dict_find_index(dict, key);
    
    if (index >= 0) {
        new_value = string_duplicate(value);
        if (new_value == NULL) {
            return -1;
        }
        free(dict->entries[index].value);
        dict->entries[index].value = new_value;
        return 0;
    }
    
    if (dict->size >= dict->capacity) {
        if (dict_resize(dict) != 0) {
            return -1;
        }
    }
    
    new_key = string_duplicate(key);
    if (new_key == NULL) {
        return -1;
    }
    
    new_value = string_duplicate(value);
    if (new_value == NULL) {
        free(new_key);
        return -1;
    }
    
    dict->entries[dict->size].key = new_key;
    dict->entries[dict->size].value = new_value;
    dict->size++;
    
    return 0;
}

const char *dict_get(const Dictionary *dict, const char *key) {
    int index;
    
    if (dict == NULL || key == NULL) {
        return NULL;
    }
    
    index = dict_find_index(dict, key);
    
    if (index >= 0) {
        return dict->entries[index].value;
    }
    
    return NULL;
}

Dictionary *dict_merge(const Dictionary *dict1, const Dictionary *dict2) {
    Dictionary *merged;
    size_t i;
    
    if (dict1 == NULL || dict2 == NULL) {
        return NULL;
    }
    
    merged = malloc(sizeof(Dictionary));
    if (merged == NULL) {
        return NULL;
    }
    
    dict_init(merged, dict1->size + dict2->size);
    
    for (i = 0; i < dict1->size; i++) {
        if (dict_set(merged, dict1->entries[i].key, dict1->entries[i].value) != 0) {
            dict_free(merged);
            free(merged);
            return NULL;
        }
    }
    
    for (i = 0; i < dict2->size; i++) {
        if (dict_set(merged, dict2->entries[i].key, dict2->entries[i].value) != 0) {
            dict_free(merged);
            free(merged);
            return NULL;
        }
    }
    
    return merged;
}

void dict_print(const Dictionary *dict) {
    size_t i;
    
    if (dict == NULL) {
        printf("NULL dictionary\n");
        return;
    }
    
    printf("{\n");
    for (i = 0; i < dict->size; i++) {
        printf("  \"%s\": \"%s\"", dict->entries[i].key, dict->entries[i].value);
        if (i < dict->size - 1) {
            printf(",");
        }
        printf("\n");
    }
    printf("}\n");
}

int main(void) {
    Dictionary dict1;
    Dictionary dict2;
    Dictionary *merged;
    
    dict_init(&dict1, 4);
    dict_init(&dict2, 4);
    
    dict_set(&dict1, "name", "Alice");
    dict_set(&dict1, "age", "30");
    dict_set(&dict1, "city", "New York");
    
    dict_set(&dict2, "country", "USA");
    dict_set(&dict2, "age", "31");
    dict_set(&dict2, "occupation", "Engineer");
    
    printf("Dictionary 1:\n");
    dict_print(&dict1);
    
    printf("\nDictionary 2:\n");
    dict_print(&dict2);
    
    merged = dict_merge(&dict1, &dict2);
    
    if (merged != NULL) {
        printf("\nMerged Dictionary:\n");
        dict_print(merged);
        
        dict_free(merged);
        free(merged);
    } else {
        fprintf(stderr, "Failed to merge dictionaries\n");
    }
    
    dict_free(&dict1);
    dict_free(&dict2);
    
    return EXIT_SUCCESS;
}