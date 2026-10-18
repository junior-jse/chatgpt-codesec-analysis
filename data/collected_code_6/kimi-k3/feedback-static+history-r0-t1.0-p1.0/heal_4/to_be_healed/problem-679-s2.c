/* Repair history (previous round):
 * - Flawfinder strlen was removed from dict_add() and now appears in main().
 */
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
    memcpy(dict->entries[dict->size].key, key, key_len);
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
    memcpy(dict->entries[dict->size].value, value, value_len);
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

    const char *key1 = "name";
    const char *value1 = "Alice";
    const char *key2 = "age";
    const char *value2 = "30";
    const char *key3 = "city";
    const char *value3 = "New York";

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    if (dict_add(&dict, key1, strlen(key1), value1, strlen(value1)) != 0 ||
        /* Possible weaknesses found:
         * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
         */
        dict_add(&dict, key2, strlen(key2), value2, strlen(value2)) != 0 ||
        /* Possible weaknesses found:
         * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
         */
        dict_add(&dict, key3, strlen(key3), value3, strlen(value3)) != 0) {
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