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

int dict_init(Dictionary *dict, size_t initial_capacity) {
    if (dict == NULL || initial_capacity == 0) {
        return -1;
    }
    dict->entries = calloc(initial_capacity, sizeof(DictEntry));
    if (dict->entries == NULL) {
        return -1;
    }
    dict->size = 0;
    dict->capacity = initial_capacity;
    return 0;
}

int dict_add(Dictionary *dict, const char *key, size_t key_len, const char *value, size_t value_len) {
    if (dict == NULL || key == NULL || value == NULL || key_len == 0) {
        return -1;
    }

    if (dict->size >= dict->capacity) {
        size_t new_capacity = dict->capacity * 2;
        if (new_capacity < dict->capacity) {
            return -1;
        }
        DictEntry *new_entries = realloc(dict->entries, new_capacity * sizeof(DictEntry));
        if (new_entries == NULL) {
            return -1;
        }
        dict->entries = new_entries;
        dict->capacity = new_capacity;
    }

    dict->entries[dict->size].key = malloc(key_len + 1);
    if (dict->entries[dict->size].key == NULL) {
        return -1;
    }
    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    if (memcpy(dict->entries[dict->size].key, key, key_len) == NULL) {
        free(dict->entries[dict->size].key);
        dict->entries[dict->size].key = NULL;
        return -1;
    }
    dict->entries[dict->size].key[key_len] = '\0';

    dict->entries[dict->size].value = malloc(value_len + 1);
    if (dict->entries[dict->size].value == NULL) {
        free(dict->entries[dict->size].key);
        dict->entries[dict->size].key = NULL;
        return -1;
    }
    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    if (memcpy(dict->entries[dict->size].value, value, value_len) == NULL) {
        free(dict->entries[dict->size].key);
        dict->entries[dict->size].key = NULL;
        free(dict->entries[dict->size].value);
        dict->entries[dict->size].value = NULL;
        return -1;
    }
    dict->entries[dict->size].value[value_len] = '\0';

    dict->size++;
    return 0;
}

const char *dict_get_key_by_index(const Dictionary *dict, size_t index) {
    if (dict == NULL || index >= dict->size) {
        return NULL;
    }
    return dict->entries[index].key;
}

const char *dict_get_value_by_index(const Dictionary *dict, size_t index) {
    if (dict == NULL || index >= dict->size) {
        return NULL;
    }
    return dict->entries[index].value;
}

void dict_free(Dictionary *dict) {
    if (dict == NULL) {
        return;
    }

    if (dict->entries != NULL) {
        for (size_t i = 0; i < dict->size; i++) {
            free(dict->entries[i].key);
            free(dict->entries[i].value);
        }
        free(dict->entries);
        dict->entries = NULL;
    }
    dict->size = 0;
    dict->capacity = 0;
}

int main(void) {
    Dictionary dict;

    if (dict_init(&dict, 4) != 0) {
        fprintf(stderr, "Failed to initialize dictionary\n");
        return EXIT_FAILURE;
    }

    const char key1[] = "name";
    const char value1[] = "Alice";
    const char key2[] = "age";
    const char value2[] = "30";
    const char key3[] = "city";
    const char value3[] = "New York";

    size_t key1_len = strnlen(key1, sizeof(key1));
    size_t value1_len = strnlen(value1, sizeof(value1));
    size_t key2_len = strnlen(key2, sizeof(key2));
    size_t value2_len = strnlen(value2, sizeof(value2));
    size_t key3_len = strnlen(key3, sizeof(key3));
    size_t value3_len = strnlen(value3, sizeof(value3));

    if (key1_len == sizeof(key1) || value1_len == sizeof(value1) ||
        key2_len == sizeof(key2) || value2_len == sizeof(value2) ||
        key3_len == sizeof(key3) || value3_len == sizeof(value3)) {
        fprintf(stderr, "Invalid string termination\n");
        dict_free(&dict);
        return EXIT_FAILURE;
    }

    if (dict_add(&dict, key1, key1_len, value1, value1_len) != 0 ||
        dict_add(&dict, key2, key2_len, value2, value2_len) != 0 ||
        dict_add(&dict, key3, key3_len, value3, value3_len) != 0) {
        fprintf(stderr, "Failed to add entries\n");
        dict_free(&dict);
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < dict.size; i++) {
        const char *key = dict_get_key_by_index(&dict, i);
        const char *value = dict_get_value_by_index(&dict, i);
        if (key != NULL && value != NULL) {
            printf("Index %zu: %s = %s\n", i, key, value);
        }
    }

    dict_free(&dict);
    return EXIT_SUCCESS;
}