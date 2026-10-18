#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>

typedef struct {
    char *key;
    int value;
} DictEntry;

typedef struct {
    DictEntry *entries;
    size_t size;
    size_t capacity;
} Dictionary;

Dictionary *dict_create(size_t capacity) {
    Dictionary *dict = malloc(sizeof(Dictionary));
    if (dict == NULL) {
        return NULL;
    }
    dict->entries = malloc(sizeof(DictEntry) * capacity);
    if (dict->entries == NULL) {
        free(dict);
        return NULL;
    }
    dict->size = 0;
    dict->capacity = capacity;
    return dict;
}

bool dict_insert(Dictionary *dict, const char *key, int value) {
    if (dict == NULL || dict->size >= dict->capacity) {
        return false;
    }
    char *key_copy = strdup(key);
    if (key_copy == NULL) {
        return false;
    }
    dict->entries[dict->size].key = key_copy;
    dict->entries[dict->size].value = value;
    dict->size++;
    return true;
}

void dict_destroy(Dictionary *dict) {
    if (dict == NULL) {
        return;
    }
    for (size_t i = 0; i < dict->size; i++) {
        free(dict->entries[i].key);
    }
    free(dict->entries);
    free(dict);
}

bool dict_all_values_same(Dictionary *dict) {
    if (dict == NULL || dict->size == 0) {
        return true;
    }
    int first_value = dict->entries[0].value;
    for (size_t i = 1; i < dict->size; i++) {
        if (dict->entries[i].value != first_value) {
            return false;
        }
    }
    return true;
}

int main(void) {
    Dictionary *dict1 = dict_create(10);
    if (dict1 == NULL) {
        return EXIT_FAILURE;
    }

    dict_insert(dict1, "a", 5);
    dict_insert(dict1, "b", 5);
    dict_insert(dict1, "c", 5);

    bool result1 = dict_all_values_same(dict1);
    printf("%d\n", result1);

    Dictionary *dict2 = dict_create(10);
    if (dict2 == NULL) {
        dict_destroy(dict1);
        return EXIT_FAILURE;
    }

    dict_insert(dict2, "x", 1);
    dict_insert(dict2, "y", 2);
    dict_insert(dict2, "z", 3);

    bool result2 = dict_all_values_same(dict2);
    printf("%d\n", result2);

    bool result3 = dict_all_values_same(NULL);
    printf("%d\n", result3);

    dict_destroy(dict1);
    dict_destroy(dict2);

    return EXIT_SUCCESS;
}