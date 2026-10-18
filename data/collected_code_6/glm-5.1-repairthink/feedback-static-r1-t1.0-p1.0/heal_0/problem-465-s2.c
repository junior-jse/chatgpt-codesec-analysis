#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *key;
    char *value;
} DictItem;

typedef struct {
    DictItem *items;
    size_t size;
    size_t capacity;
} Dictionary;

Dictionary *dict_create(size_t capacity) {
    Dictionary *dict = malloc(sizeof(Dictionary));
    if (!dict) return NULL;
    dict->items = malloc(capacity * sizeof(DictItem));
    if (!dict->items) {
        free(dict);
        return NULL;
    }
    dict->size = 0;
    dict->capacity = capacity;
    return dict;
}

int dict_insert(Dictionary *dict, const char *key, const char *value) {
    if (!dict || !key || !value) return -1;
    if (dict->size == dict->capacity) {
        size_t new_capacity = dict->capacity * 2;
        DictItem *new_items = realloc(dict->items, new_capacity * sizeof(DictItem));
        if (!new_items) return -1;
        dict->items = new_items;
        dict->capacity = new_capacity;
    }
    dict->items[dict->size].key = strdup(key);
    dict->items[dict->size].value = strdup(value);
    if (!dict->items[dict->size].key || !dict->items[dict->size].value) {
        free(dict->items[dict->size].key);
        free(dict->items[dict->size].value);
        return -1;
    }
    dict->size++;
    return 0;
}

Dictionary *dict_drop_empty(Dictionary *dict) {
    if (!dict) return NULL;
    
    size_t new_size = 0;
    for (size_t i = 0; i < dict->size; i++) {
        if (dict->items[i].value != NULL && dict->items[i].value[0] != '\0') {
            if (new_size != i) {
                dict->items[new_size] = dict->items[i];
                memset(&dict->items[i], 0, sizeof(DictItem));
            }
            new_size++;
        } else {
            free(dict->items[i].key);
            free(dict->items[i].value);
            memset(&dict->items[i], 0, sizeof(DictItem));
        }
    }
    
    dict->size = new_size;
    
    if (dict->size < dict->capacity / 2 && dict->capacity > 4) {
        size_t new_capacity = dict->capacity / 2;
        DictItem *new_items = realloc(dict->items, new_capacity * sizeof(DictItem));
        if (new_items) {
            dict->items = new_items;
            dict->capacity = new_capacity;
        }
    }
    
    return dict;
}

void dict_free(Dictionary *dict) {
    if (!dict) return;
    for (size_t i = 0; i < dict->size; i++) {
        free(dict->items[i].key);
        free(dict->items[i].value);
    }
    free(dict->items);
    free(dict);
}

void dict_print(const Dictionary *dict) {
    if (!dict) return;
    printf("{\n");
    for (size_t i = 0; i < dict->size; i++) {
        printf("  \"%s\": \"%s\",\n", dict->items[i].key, dict->items[i].value);
    }
    printf("}\n");
}

int main(void) {
    Dictionary *dict = dict_create(10);
    if (!dict) return EXIT_FAILURE;
    
    dict_insert(dict, "a", "1");
    dict_insert(dict, "b", "");
    dict_insert(dict, "c", "3");
    dict_insert(dict, "d", NULL);
    dict_insert(dict, "e", "5");
    
    printf("Before:\n");
    dict_print(dict);
    
    Dictionary *cleaned_dict = dict_drop_empty(dict);
    if (!cleaned_dict) {
        dict_free(dict);
        return EXIT_FAILURE;
    }
    
    printf("After:\n");
    dict_print(cleaned_dict);
    
    dict_free(cleaned_dict);
    return EXIT_SUCCESS;
}