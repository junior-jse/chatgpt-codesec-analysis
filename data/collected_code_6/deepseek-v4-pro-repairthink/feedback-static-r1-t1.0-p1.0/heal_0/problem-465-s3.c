#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 16

typedef struct {
    char *key;
    char *value;
} DictItem;

typedef struct {
    DictItem *items;
    size_t size;
    size_t capacity;
} Dictionary;

Dictionary *create_dictionary(void) {
    Dictionary *dict = malloc(sizeof(*dict));
    if (!dict) return NULL;
    
    dict->items = malloc(INITIAL_CAPACITY * sizeof(DictItem));
    if (!dict->items) {
        free(dict);
        return NULL;
    }
    
    dict->size = 0;
    dict->capacity = INITIAL_CAPACITY;
    return dict;
}

void destroy_dictionary(Dictionary *dict) {
    if (!dict) return;
    
    for (size_t i = 0; i < dict->size; i++) {
        free(dict->items[i].key);
        free(dict->items[i].value);
    }
    
    free(dict->items);
    free(dict);
}

int dictionary_add(Dictionary *dict, const char *key, const char *value) {
    if (!dict || !key || !value) return 0;
    
    if (dict->size == dict->capacity) {
        size_t new_capacity = dict->capacity * 2;
        DictItem *new_items = realloc(dict->items, new_capacity * sizeof(DictItem));
        if (!new_items) return 0;
        
        dict->items = new_items;
        dict->capacity = new_capacity;
    }
    
    char *key_copy = malloc(strlen(key) + 1);
    char *value_copy = malloc(strlen(value) + 1);
    
    if (!key_copy || !value_copy) {
        free(key_copy);
        free(value_copy);
        return 0;
    }
    
    strcpy(key_copy, key);
    strcpy(value_copy, value);
    
    dict->items[dict->size].key = key_copy;
    dict->items[dict->size].value = value_copy;
    dict->size++;
    
    return 1;
}

void dictionary_remove_empty(Dictionary *dict) {
    if (!dict) return;
    
    size_t write_index = 0;
    
    for (size_t read_index = 0; read_index < dict->size; read_index++) {
        if (dict->items[read_index].value[0] == '\0') {
            free(dict->items[read_index].key);
            free(dict->items[read_index].value);
        } else {
            dict->items[write_index] = dict->items[read_index];
            write_index++;
        }
    }
    
    dict->size = write_index;
}

void dictionary_print(const Dictionary *dict) {
    if (!dict) return;
    
    for (size_t i = 0; i < dict->size; i++) {
        printf("%s: %s\n", dict->items[i].key, dict->items[i].value);
    }
}

int main(void) {
    Dictionary *dict = create_dictionary();
    if (!dict) return EXIT_FAILURE;
    
    printf("Original dictionary:\n");
    dictionary_add(dict, "key1", "value1");
    dictionary_add(dict, "key2", "");
    dictionary_add(dict, "key3", "value3");
    dictionary_add(dict, "key4", "");
    dictionary_add(dict, "key5", "value5");
    dictionary_print(dict);
    
    dictionary_remove_empty(dict);
    
    printf("\nAfter removing empty items:\n");
    dictionary_print(dict);
    
    destroy_dictionary(dict);
    
    return EXIT_SUCCESS;
}