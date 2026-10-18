#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define DICT_MAX_STRING_LENGTH 4096

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
    if (initial_capacity > SIZE_MAX / sizeof(DictEntry)) {
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

static char *dict_strdup_safe(const char *src) {
    size_t length;
    size_t dest_size;
    char *dest;
    int written;

    if (src == NULL) {
        return NULL;
    }
    length = strnlen(src, DICT_MAX_STRING_LENGTH);
    if (length >= DICT_MAX_STRING_LENGTH) {
        return NULL;
    }
    dest_size = length + 1;
    dest = malloc(dest_size);
    if (dest == NULL) {
        return NULL;
    }
    written = snprintf(dest, dest_size, "%s", src);
    if (written < 0 || (size_t)written >= dest_size) {
        free(dest);
        return NULL;
    }
    return dest;
}

int dict_add(Dictionary *dict, const char *key, const char *value) {
    /* Possible weaknesses found:
     *  The scope of the variable 'new_capacity' can be reduced. [variableScope]
     */
    size_t new_capacity;
    DictEntry *new_entries;

    if (dict == NULL || key == NULL || value == NULL) {
        return -1;
    }
    if (dict->size >= dict->capacity) {
        if (dict->capacity > SIZE_MAX / 2) {
            return -1;
        }
        new_capacity = dict->capacity * 2;
        if (new_capacity > SIZE_MAX / sizeof(DictEntry)) {
            return -1;
        }
        new_entries = realloc(dict->entries, new_capacity * sizeof(DictEntry));
        if (new_entries == NULL) {
            return -1;
        }
        dict->entries = new_entries;
        dict->capacity = new_capacity;
    }
    dict->entries[dict->size].key = dict_strdup_safe(key);
    if (dict->entries[dict->size].key == NULL) {
        return -1;
    }
    dict->entries[dict->size].value = dict_strdup_safe(value);
    if (dict->entries[dict->size].value == NULL) {
        free(dict->entries[dict->size].key);
        dict->entries[dict->size].key = NULL;
        return -1;
    }
    dict->size++;
    return 0;
}

const char *dict_get_key_by_index(const Dictionary *dict, size_t index) {
    if (dict == NULL || dict->entries == NULL || index >= dict->size) {
        return NULL;
    }
    return dict->entries[index].key;
}

const char *dict_get_value_by_index(const Dictionary *dict, size_t index) {
    if (dict == NULL || dict->entries == NULL || index >= dict->size) {
        return NULL;
    }
    return dict->entries[index].value;
}

void dict_free(Dictionary *dict) {
    if (dict == NULL) {
        return;
    }
    for (size_t i = 0; i < dict->size; i++) {
        free(dict->entries[i].key);
        free(dict->entries[i].value);
        dict->entries[i].key = NULL;
        dict->entries[i].value = NULL;
    }
    free(dict->entries);
    dict->entries = NULL;
    dict->size = 0;
    dict->capacity = 0;
}

int main(void) {
    Dictionary dict;
    if (dict_init(&dict, 4) != 0) {
        fprintf(stderr, "Failed to initialize dictionary\n");
        return EXIT_FAILURE;
    }

    if (dict_add(&dict, "name", "John") != 0 ||
        dict_add(&dict, "age", "30") != 0 ||
        dict_add(&dict, "city", "New York") != 0) {
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