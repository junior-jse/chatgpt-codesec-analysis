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
    if (!dict || dict->size >= dict->capacity) return -1;
    dict->items[dict->size].key = key ? strdup(key) : NULL;
    dict->items[dict->size].value = value ? strdup(value) : NULL;
    if ((key && !dict->items[dict->size].key) || (value && !dict->items[dict->size].value)) {
        free(dict->items[dict->size].key);
        free(dict->items[dict->size].value);
        return -1;
    }
    dict->size++;
    return 0;
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

Dictionary *dict_drop_empty(Dictionary *dict) {
    if (!dict) return NULL;
    size_t valid_count = 0;
    for (size_t i = 0; i < dict->size; i++) {
        if (dict->items[i].value != NULL && dict->items[i].value[0] != '\0') {
            valid_count++;
        }
    }
    Dictionary *new_dict = dict_create(valid_count > 0 ? valid_count : 1);
    if (!new_dict) return NULL;
    for (size_t i = 0; i < dict->size; i++) {
        if (dict->items[i].value != NULL && dict->items[i].value[0] != '\0') {
            if (dict_insert(new_dict, dict->items[i].key, dict->items[i].value) != 0) {
                dict_free(new_dict);
                return NULL;
            }
        }
    }
    return new_dict;
}

int main(void) {
    Dictionary *dict = dict_create(4);
    if (!dict) return 1;
    dict_insert(dict, "a", "1");
    dict_insert(dict, "b", "");
    dict_insert(dict, "c", NULL);
    dict_insert(dict, "d", "4");
    Dictionary *filtered = dict_drop_empty(dict);
    if (filtered) {
        for (size_t i = 0; i < filtered->size; i++) {
            printf("%s: %s\n", filtered->items[i].key, filtered->items[i].value);
        }
        dict_free(filtered);
    }
    dict_free(dict);
    return 0;
}