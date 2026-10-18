#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KEY_SIZE 256
#define VALUE_SIZE 256

typedef struct {
    char *key;
    char *value;
} DictEntry;

typedef struct {
    DictEntry *entries;
    size_t size;
    size_t capacity;
} Dictionary;

int dict_init(Dictionary *dict, size_t capacity) {
    if (dict == NULL) {
        return -1;
    }
    dict->entries = NULL;
    dict->size = 0;
    dict->capacity = 0;

    if (capacity > 0) {
        dict->entries = (DictEntry *)calloc(capacity, sizeof(DictEntry));
        if (dict->entries == NULL) {
            return -1;
        }
        dict->capacity = capacity;
    }
    return 0;
}

void dict_free(Dictionary *dict) {
    if (dict != NULL && dict->entries != NULL) {
        for (size_t i = 0; i < dict->capacity; i++) {
            free(dict->entries[i].key);
            free(dict->entries[i].value);
        }
        free(dict->entries);
        dict->entries = NULL;
        dict->size = 0;
        dict->capacity = 0;
    }
}

int dict_is_empty(const Dictionary *dict) {
    if (dict == NULL) {
        return 1;
    }
    return dict->size == 0;
}

int all_dicts_empty(const Dictionary *dicts, size_t count) {
    if (dicts == NULL || count == 0) {
        return 1;
    }

    for (size_t i = 0; i < count; i++) {
        if (!dict_is_empty(&dicts[i])) {
            return 0;
        }
    }
    return 1;
}

int dict_set_entry(Dictionary *dict, size_t index, const char *key, const char *value) {
    if (dict == NULL || dict->entries == NULL || key == NULL || value == NULL) {
        return -1;
    }
    if (index >= dict->capacity) {
        return -1;
    }

    size_t key_len = strnlen(key, KEY_SIZE);
    size_t value_len = strnlen(value, VALUE_SIZE);

    if (key_len >= KEY_SIZE || value_len >= VALUE_SIZE) {
        return -1;
    }

    char *new_key = (char *)malloc(key_len + 1);
    if (new_key == NULL) {
        return -1;
    }

    char *new_value = (char *)malloc(value_len + 1);
    if (new_value == NULL) {
        free(new_key);
        return -1;
    }

    strncpy(new_key, key, key_len + 1);
    strncpy(new_value, value, value_len + 1);

    free(dict->entries[index].key);
    free(dict->entries[index].value);

    dict->entries[index].key = new_key;
    dict->entries[index].value = new_value;

    if (index >= dict->size) {
        dict->size = index + 1;
    }

    return 0;
}

int main(void) {
    Dictionary *dict_list = NULL;
    size_t dict_count = 3;
    int result;

    dict_list = (Dictionary *)calloc(dict_count, sizeof(Dictionary));
    if (dict_list == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < dict_count; i++) {
        if (dict_init(&dict_list[i], 10) != 0) {
            for (size_t j = 0; j < i; j++) {
                dict_free(&dict_list[j]);
            }
            free(dict_list);
            fprintf(stderr, "Dictionary initialization failed\n");
            return EXIT_FAILURE;
        }
    }

    result = all_dicts_empty(dict_list, dict_count);
    printf("All dictionaries empty: %s\n", result ? "true" : "false");

    if (dict_set_entry(&dict_list[1], 0, "test", "") != 0) {
        fprintf(stderr, "Failed to set dictionary entry\n");
    }

    result = all_dicts_empty(dict_list, dict_count);
    printf("All dictionaries empty: %s\n", result ? "true" : "false");

    for (size_t i = 0; i < dict_count; i++) {
        dict_free(&dict_list[i]);
    }
    free(dict_list);

    return EXIT_SUCCESS;
}