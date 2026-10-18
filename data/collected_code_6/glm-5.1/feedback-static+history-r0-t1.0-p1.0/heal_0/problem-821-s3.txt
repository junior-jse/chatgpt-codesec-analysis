#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char **keys;
    char **values;
    size_t size;
    size_t capacity;
} Dictionary;

Dictionary *dict_create(size_t capacity) {
    Dictionary *dict = malloc(sizeof(Dictionary));
    if (!dict) return NULL;
    dict->keys = malloc(capacity * sizeof(char *));
    dict->values = malloc(capacity * sizeof(char *));
    if (!dict->keys || !dict->values) {
        free(dict->keys);
        free(dict->values);
        free(dict);
        return NULL;
    }
    dict->size = 0;
    dict->capacity = capacity;
    return dict;
}

void dict_free(Dictionary *dict) {
    if (!dict) return;
    for (size_t i = 0; i < dict->size; i++) {
        free(dict->keys[i]);
        free(dict->values[i]);
    }
    free(dict->keys);
    free(dict->values);
    free(dict);
}

size_t dict_find(Dictionary *dict, const char *key) {
    for (size_t i = 0; i < dict->size; i++) {
        if (strcmp(dict->keys[i], key) == 0) return i;
    }
    return dict->size;
}

int dict_set(Dictionary *dict, const char *key, const char *value) {
    size_t idx = dict_find(dict, key);
    if (idx < dict->size) {
        char *new_val = strdup(value);
        if (!new_val) return -1;
        free(dict->values[idx]);
        dict->values[idx] = new_val;
        return 0;
    }
    if (dict->size >= dict->capacity) return -1;
    char *k = strdup(key);
    char *v = strdup(value);
    if (!k || !v) {
        free(k);
        free(v);
        return -1;
    }
    dict->keys[dict->size] = k;
    dict->values[dict->size] = v;
    dict->size++;
    return 0;
}

Dictionary *dict_merge(Dictionary *d1, Dictionary *d2) {
    if (!d1 || !d2) return NULL;
    size_t max_size = d1->size + d2->size;
    Dictionary *merged = dict_create(max_size);
    if (!merged) return NULL;
    for (size_t i = 0; i < d1->size; i++) {
        if (dict_set(merged, d1->keys[i], d1->values[i]) != 0) {
            dict_free(merged);
            return NULL;
        }
    }
    for (size_t i = 0; i < d2->size; i++) {
        if (dict_set(merged, d2->keys[i], d2->values[i]) != 0) {
            dict_free(merged);
            return NULL;
        }
    }
    return merged;
}

void dict_print(Dictionary *dict) {
    if (!dict) {
        printf("NULL\n");
        return;
    }
    printf("{");
    for (size_t i = 0; i < dict->size; i++) {
        printf("\"%s\": \"%s\"", dict->keys[i], dict->values[i]);
        if (i < dict->size - 1) printf(", ");
    }
    printf("}\n");
}

int main(void) {
    Dictionary *d1 = dict_create(10);
    if (!d1) return 1;
    Dictionary *d2 = dict_create(10);
    if (!d2) {
        dict_free(d1);
        return 1;
    }
    dict_set(d1, "a", "1");
    dict_set(d1, "b", "2");
    dict_set(d2, "b", "3");
    dict_set(d2, "c", "4");
    Dictionary *merged = dict_merge(d1, d2);
    dict_print(merged);
    dict_free(d1);
    dict_free(d2);
    dict_free(merged);
    return 0;
}