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

Dictionary *dict_create(size_t capacity) {
    Dictionary *dict = malloc(sizeof(Dictionary));
    if (!dict) return NULL;
    dict->entries = malloc(capacity * sizeof(DictEntry));
    if (!dict->entries) {
        free(dict);
        return NULL;
    }
    dict->size = 0;
    dict->capacity = capacity;
    return dict;
}

void dict_destroy(Dictionary *dict) {
    if (!dict) return;
    for (size_t i = 0; i < dict->size; i++) {
        free(dict->entries[i].key);
        free(dict->entries[i].value);
    }
    free(dict->entries);
    free(dict);
}

int dict_insert(Dictionary *dict, const char *key, const char *value) {
    if (!dict || !key || !value) return -1;
    if (dict->size >= dict->capacity) {
        size_t new_capacity = dict->capacity * 2;
        DictEntry *new_entries = realloc(dict->entries, new_capacity * sizeof(DictEntry));
        if (!new_entries) return -1;
        dict->entries = new_entries;
        dict->capacity = new_capacity;
    }
    char *k = strdup(key);
    if (!k) return -1;
    char *v = strdup(value);
    if (!v) {
        free(k);
        return -1;
    }
    dict->entries[dict->size].key = k;
    dict->entries[dict->size].value = v;
    dict->size++;
    return 0;
}

Dictionary *dict_merge(const Dictionary *d1, const Dictionary *d2) {
    if (!d1 || !d2) return NULL;
    size_t new_capacity = d1->size + d2->size;
    if (new_capacity == 0) new_capacity = 1;
    Dictionary *merged = dict_create(new_capacity);
    if (!merged) return NULL;
    for (size_t i = 0; i < d1->size; i++) {
        if (dict_insert(merged, d1->entries[i].key, d1->entries[i].value) != 0) {
            dict_destroy(merged);
            return NULL;
        }
    }
    for (size_t i = 0; i < d2->size; i++) {
        int exists = 0;
        for (size_t j = 0; j < d1->size; j++) {
            if (strcmp(d2->entries[i].key, d1->entries[j].key) == 0) {
                exists = 1;
                break;
            }
        }
        if (!exists) {
            if (dict_insert(merged, d2->entries[i].key, d2->entries[i].value) != 0) {
                dict_destroy(merged);
                return NULL;
            }
        }
    }
    return merged;
}

int main(void) {
    Dictionary *d1 = dict_create(4);
    Dictionary *d2 = dict_create(4);
    if (!d1 || !d2) {
        dict_destroy(d1);
        dict_destroy(d2);
        return 1;
    }
    dict_insert(d1, "A", "1");
    dict_insert(d1, "B", "2");
    dict_insert(d2, "B", "3");
    dict_insert(d2, "C", "4");
    Dictionary *merged = dict_merge(d1, d2);
    if (!merged) {
        dict_destroy(d1);
        dict_destroy(d2);
        return 1;
    }
    for (size_t i = 0; i < merged->size; i++) {
        printf("%s: %s\n", merged->entries[i].key, merged->entries[i].value);
    }
    dict_destroy(d1);
    dict_destroy(d2);
    dict_destroy(merged);
    return 0;
}