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

int dict_add(Dictionary *dict, const char *key, const char *value) {
    DictEntry *new_entries;
    /* Possible weaknesses found:
     *  The scope of the variable 'new_capacity' can be reduced. [variableScope]
     */
    size_t new_capacity;

    if (dict == NULL || key == NULL || value == NULL) {
        return -1;
    }

    if (dict->size >= dict->capacity) {
        new_capacity = dict->capacity * 2;
        new_entries = realloc(dict->entries, new_capacity * sizeof(DictEntry));
        if (new_entries == NULL) {
            return -1;
        }
        dict->entries = new_entries;
        dict->capacity = new_capacity;
    }

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    dict->entries[dict->size].key = malloc(strlen(key) + 1);
    if (dict->entries[dict->size].key == NULL) {
        return -1;
    }
    /* Possible weaknesses found:
     * Flawfinder strcpy: Does not check for buffer overflows when copying to destination [MS-banned] (CWE-120). Consider using snprintf, strcpy_s, or strlcpy (warning: strncpy easily misused). (risk 4, buffer)
     */
    strcpy(dict->entries[dict->size].key, key);

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    dict->entries[dict->size].value = malloc(strlen(value) + 1);
    if (dict->entries[dict->size].value == NULL) {
        free(dict->entries[dict->size].key);
        dict->entries[dict->size].key = NULL;
        return -1;
    }
    /* Possible weaknesses found:
     * Flawfinder strcpy: Does not check for buffer overflows when copying to destination [MS-banned] (CWE-120). Consider using snprintf, strcpy_s, or strlcpy (warning: strncpy easily misused). (risk 4, buffer)
     */
    strcpy(dict->entries[dict->size].value, value);

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
    /* Possible weaknesses found:
     *  The scope of the variable 'i' can be reduced. [variableScope]
     */
    size_t i;

    if (dict == NULL) {
        return;
    }

    if (dict->entries != NULL) {
        for (i = 0; i < dict->size; i++) {
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
    size_t i;
    /* Possible weaknesses found:
     *  The scope of the variable 'key' can be reduced. [variableScope]
     */
    const char *key;
    /* Possible weaknesses found:
     *  The scope of the variable 'value' can be reduced. [variableScope]
     */
    const char *value;

    if (dict_init(&dict, 4) != 0) {
        fprintf(stderr, "Failed to initialize dictionary\n");
        return EXIT_FAILURE;
    }

    if (dict_add(&dict, "name", "Alice") != 0 ||
        dict_add(&dict, "age", "30") != 0 ||
        dict_add(&dict, "city", "New York") != 0) {
        fprintf(stderr, "Failed to add entries\n");
        dict_free(&dict);
        return EXIT_FAILURE;
    }

    for (i = 0; i < dict.size; i++) {
        key = dict_get_key_by_index(&dict, i);
        value = dict_get_value_by_index(&dict, i);
        if (key != NULL && value != NULL) {
            printf("Index %zu: %s = %s\n", i, key, value);
        }
    }

    dict_free(&dict);
    return EXIT_SUCCESS;
}