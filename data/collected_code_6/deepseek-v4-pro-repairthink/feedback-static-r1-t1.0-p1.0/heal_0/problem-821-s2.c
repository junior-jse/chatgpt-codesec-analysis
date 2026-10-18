#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *key;
    char *value;
} Entry;

typedef struct {
    Entry *entries;
    size_t size;
    size_t capacity;
} Dictionary;

Dictionary *dict_create(void) {
    Dictionary *dict = malloc(sizeof(Dictionary));
    if (dict == NULL) return NULL;
    dict->entries = malloc(sizeof(Entry) * 8);
    if (dict->entries == NULL) {
        free(dict);
        return NULL;
    }
    dict->size = 0;
    dict->capacity = 8;
    return dict;
}

int dict_add(Dictionary *dict, const char *key, const char *value) {
    if (dict == NULL || key == NULL || value == NULL) return -1;
    
    char *key_copy = strdup(key);
    char *value_copy = strdup(value);
    if (key_copy == NULL || value_copy == NULL) {
        free(key_copy);
        free(value_copy);
        return -1;
    }
    
    if (dict->size >= dict->capacity) {
        size_t new_capacity = dict->capacity * 2;
        Entry *new_entries = realloc(dict->entries, sizeof(Entry) * new_capacity);
        if (new_entries == NULL) {
            free(key_copy);
            free(value_copy);
            return -1;
        }
        dict->entries = new_entries;
        dict->capacity = new_capacity;
    }
    
    dict->entries[dict->size].key = key_copy;
    dict->entries[dict->size].value = value_copy;
    dict->size++;
    return 0;
}

const char *dict_get(const Dictionary *dict, const char *key) {
    if (dict == NULL || key == NULL) return NULL;
    
    for (size_t i = 0; i < dict->size; i++) {
        if (strcmp(dict->entries[i].key, key) == 0) {
            return dict->entries[i].value;
        }
    }
    return NULL;
}

Dictionary *dict_merge(const Dictionary *dict1, const Dictionary *dict2) {
    if (dict1 == NULL || dict2 == NULL) return NULL;
    
    Dictionary *merged = dict_create();
    if (merged == NULL) return NULL;
    
    for (size_t i = 0; i < dict1->size; i++) {
        if (dict_add(merged, dict1->entries[i].key, dict1->entries[i].value) != 0) {
            dict_free(merged);
            return NULL;
        }
    }
    
    for (size_t i = 0; i < dict2->size; i++) {
        const char *existing_value = dict_get(merged, dict2->entries[i].key);
        if (existing_value != NULL) {
            for (size_t j = 0; j < merged->size; j++) {
                if (strcmp(merged->entries[j].key, dict2->entries[i].key) == 0) {
                    char *new_value = strdup(dict2->entries[i].value);
                    if (new_value == NULL) {
                        dict_free(merged);
                        return NULL;
                    }
                    free(merged->entries[j].value);
                    merged->entries[j].value = new_value;
                    break;
                }
            }
        } else {
            if (dict_add(merged, dict2->entries[i].key, dict2->entries[i].value) != 0) {
                dict_free(merged);
                return NULL;
            }
        }
    }
    
    return merged;
}

void dict_free(Dictionary *dict) {
    if (dict == NULL) return;
    
    for (size_t i = 0; i < dict->size; i++) {
        free(dict->entries[i].key);
        free(dict->entries[i].value);
    }
    free(dict->entries);
    free(dict);
}

void dict_print(const Dictionary *dict) {
    if (dict == NULL) return;
    
    for (size_t i = 0; i < dict->size; i++) {
        printf("%s: %s\n", dict->entries[i].key, dict->entries[i].value);
    }
}

int main(void) {
    Dictionary *dict1 = dict_create();
    Dictionary *dict2 = dict_create();
    
    if (dict1 == NULL || dict2 == NULL) {
        dict_free(dict1);
        dict_free(dict2);
        return 1;
    }
    
    dict_add(dict1, "name", "Alice");
    dict_add(dict1, "age", "30");
    dict_add(dict1, "city", "New York");
    
    dict_add(dict2, "country", "USA");
    dict_add(dict2, "age", "31");
    dict_add(dict2, "occupation", "Engineer");
    
    Dictionary *merged = dict_merge(dict1, dict2);
    if (merged == NULL) {
        dict_free(dict1);
        dict_free(dict2);
        return 1;
    }
    
    dict_print(merged);
    
    dict_free(merged);
    dict_free(dict1);
    dict_free(dict2);
    
    return 0;
}