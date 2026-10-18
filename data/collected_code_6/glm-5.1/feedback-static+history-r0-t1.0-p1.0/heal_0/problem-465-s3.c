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

Dictionary dictionary_create(size_t capacity) {
    Dictionary dict;
    dict.items = (DictItem *)malloc(capacity * sizeof(DictItem));
    dict.size = 0;
    dict.capacity = capacity;
    return dict;
}

void dictionary_insert(Dictionary *dict, const char *key, const char *value) {
    if (dict->size == dict->capacity) {
        size_t new_capacity = dict->capacity == 0 ? 1 : dict->capacity * 2;
        DictItem *new_items = (DictItem *)realloc(dict->items, new_capacity * sizeof(DictItem));
        if (new_items == NULL) {
            return;
        }
        dict->items = new_items;
        dict->capacity = new_capacity;
    }
    dict->items[dict->size].key = key ? strdup(key) : NULL;
    dict->items[dict->size].value = value ? strdup(value) : NULL;
    dict->size++;
}

void dictionary_free(Dictionary *dict) {
    for (size_t i = 0; i < dict->size; i++) {
        free(dict->items[i].key);
        free(dict->items[i].value);
    }
    free(dict->items);
    dict->items = NULL;
    dict->size = 0;
    dict->capacity = 0;
}

Dictionary dictionary_drop_empty(Dictionary *dict) {
    Dictionary result = dictionary_create(dict->size);
    for (size_t i = 0; i < dict->size; i++) {
        if (dict->items[i].value != NULL && dict->items[i].value[0] != '\0') {
            dictionary_insert(&result, dict->items[i].key, dict->items[i].value);
        }
    }
    return result;
}

int main(void) {
    Dictionary dict = dictionary_create(4);
    dictionary_insert(&dict, "a", "1");
    dictionary_insert(&dict, "b", "");
    dictionary_insert(&dict, "c", NULL);
    dictionary_insert(&dict, "d", "4");

    Dictionary filtered = dictionary_drop_empty(&dict);

    for (size_t i = 0; i < filtered.size; i++) {
        printf("%s: %s\n", filtered.items[i].key, filtered.items[i].value);
    }

    dictionary_free(&dict);
    dictionary_free(&filtered);

    return 0;
}