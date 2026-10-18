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
    if (dict == NULL) {
        return NULL;
    }
    dict->entries = malloc(capacity * sizeof(DictEntry));
    if (dict->entries == NULL) {
        free(dict);
        return NULL;
    }
    dict->size = 0;
    dict->capacity = capacity;
    return dict;
}

void dict_free(Dictionary *dict) {
    if (dict == NULL) {
        return;
    }
    for (size_t i = 0; i < dict->size; i++) {
        free(dict->entries[i].key);
        free(dict->entries[i].value);
    }
    free(dict->entries);
    free(dict);
}

static char *string_duplicate(const char *str) {
    if (str == NULL) {
        return NULL;
    }
    size_t len = strlen(str) + 1;
    char *copy = malloc(len);
    if (copy == NULL) {
        return NULL;
    }
    memcpy(copy, str, len);
    return copy;
}

int dict_add(Dictionary *dict, const char *key, const char *value) {
    if (dict == NULL || key == NULL || value == NULL) {
        return -1;
    }
    if (dict->size >= dict->capacity) {
        return -1;
    }
    char *key_copy = string_duplicate(key);
    if (key_copy == NULL) {
        return -1;
    }
    char *value_copy = string_duplicate(value);
    if (value_copy == NULL) {
        free(key_copy);
        return -1;
    }
    dict->entries[dict->size].key = key_copy;
    dict->entries[dict->size].value = value_copy;
    dict->size++;
    return 0;
}

static ssize_t dict_find(const Dictionary *dict, const char *key) {
    if (dict == NULL || key == NULL) {
        return -1;
    }
    for (size_t i = 0; i < dict->size; i++) {
        if (strcmp(dict->entries[i].key, key) == 0) {
            return (ssize_t)i;
        }
    }
    return -1;
}

Dictionary *dict_merge(const Dictionary *dict1, const Dictionary *dict2) {
    if (dict1 == NULL || dict2 == NULL) {
        return NULL;
    }
    size_t total_capacity = dict1->size + dict2->size;
    Dictionary *merged = dict_create(total_capacity);
    if (merged == NULL) {
        return NULL;
    }
    for (size_t i = 0; i < dict1->size; i++) {
        if (dict_add(merged, dict1->entries[i].key, dict1->entries[i].value) != 0) {
            dict_free(merged);
            return NULL;
        }
    }
    for (size_t i = 0; i < dict2->size; i++) {
        ssize_t idx = dict_find(merged, dict2->entries[i].key);
        if (idx >= 0) {
            char *new_value = string_duplicate(dict2->entries[i].value);
            if (new_value == NULL) {
                dict_free(merged);
                return NULL;
            }
            free(merged->entries[idx].value);
            merged->entries[idx].value = new_value;
        } else {
            if (dict_add(merged, dict2->entries[i].key, dict2->entries[i].value) != 0) {
                dict_free(merged);
                return NULL;
            }
        }
    }
    return merged;
}

void dict_print(const Dictionary *dict) {
    if (dict == NULL) {
        printf("NULL dictionary\n");
        return;
    }
    printf("{");
    for (size_t i = 0; i < dict->size; i++) {
        printf("\"%s\": \"%s\"", dict->entries[i].key, dict->entries[i].value);
        if (i < dict->size - 1) {
            printf(", ");
        }
    }
    printf("}\n");
}

int main(void) {
    Dictionary *dict1 = dict_create(10);
    if (dict1 == NULL) {
        fprintf(stderr, "Failed to create dict1\n");
        return EXIT_FAILURE;
    }
    Dictionary *dict2 = dict_create(10);
    if (dict2 == NULL) {
        fprintf(stderr, "Failed to create dict2\n");
        dict_free(dict1);
        return EXIT_FAILURE;
    }
    if (dict_add(dict1, "name", "Alice") != 0 ||
        dict_add(dict1, "age", "30") != 0 ||
        dict_add(dict1, "city", "New York") != 0) {
        fprintf(stderr, "Failed to add entries to dict1\n");
        dict_free(dict1);
        dict_free(dict2);
        return EXIT_FAILURE;
    }
    if (dict_add(dict2, "age", "31") != 0 ||
        dict_add(dict2, "country", "USA") != 0 ||
        dict_add(dict2, "occupation", "Engineer") != 0) {
        fprintf(stderr, "Failed to add entries to dict2\n");
        dict_free(dict1);
        dict_free(dict2);
        return EXIT_FAILURE;
    }
    printf("Dictionary 1: ");
    dict_print(dict1);
    printf("Dictionary 2: ");
    dict_print(dict2);
    Dictionary *merged = dict_merge(dict1, dict2);
    if (merged == NULL) {
        fprintf(stderr, "Failed to merge dictionaries\n");
        dict_free(dict1);
        dict_free(dict2);
        return EXIT_FAILURE;
    }
    printf("Merged: ");
    dict_print(merged);
    dict_free(dict1);
    dict_free(dict2);
    dict_free(merged);
    return EXIT_SUCCESS;
}