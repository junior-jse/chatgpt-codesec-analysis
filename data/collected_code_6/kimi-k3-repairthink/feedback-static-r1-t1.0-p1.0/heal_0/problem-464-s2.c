#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_KEY_LEN 64
#define INITIAL_CAPACITY 16

typedef struct {
    char key[MAX_KEY_LEN];
    int value;
    bool is_occupied;
} DictEntry;

typedef struct {
    DictEntry *entries;
    size_t size;
    size_t capacity;
} Dictionary;

bool dict_init(Dictionary *dict) {
    if (dict == NULL) {
        return false;
    }
    dict->entries = calloc(INITIAL_CAPACITY, sizeof(DictEntry));
    if (dict->entries == NULL) {
        return false;
    }
    dict->size = 0;
    dict->capacity = INITIAL_CAPACITY;
    return true;
}

void dict_free(Dictionary *dict) {
    if (dict != NULL && dict->entries != NULL) {
        free(dict->entries);
        dict->entries = NULL;
        dict->size = 0;
        dict->capacity = 0;
    }
}

static size_t hash_string(const char *str) {
    size_t hash = 5381;
    int c;
    while ((c = *str++) != '\0') {
        hash = ((hash << 5) + hash) + (size_t)c;
    }
    return hash;
}

static bool dict_resize(Dictionary *dict) {
    size_t new_capacity = dict->capacity * 2;
    DictEntry *new_entries = calloc(new_capacity, sizeof(DictEntry));
    if (new_entries == NULL) {
        return false;
    }

    for (size_t i = 0; i < dict->capacity; i++) {
        if (dict->entries[i].is_occupied) {
            size_t idx = hash_string(dict->entries[i].key) % new_capacity;
            while (new_entries[idx].is_occupied) {
                idx = (idx + 1) % new_capacity;
            }
            new_entries[idx] = dict->entries[i];
        }
    }

    free(dict->entries);
    dict->entries = new_entries;
    dict->capacity = new_capacity;
    return true;
}

bool dict_insert(Dictionary *dict, const char *key, int value) {
    if (dict == NULL || key == NULL || strlen(key) >= MAX_KEY_LEN) {
        return false;
    }

    if (dict->size >= dict->capacity * 3 / 4) {
        if (!dict_resize(dict)) {
            return false;
        }
    }

    size_t idx = hash_string(key) % dict->capacity;
    size_t start_idx = idx;

    while (dict->entries[idx].is_occupied) {
        if (strcmp(dict->entries[idx].key, key) == 0) {
            dict->entries[idx].value = value;
            return true;
        }
        idx = (idx + 1) % dict->capacity;
        if (idx == start_idx) {
            return false;
        }
    }

    strncpy(dict->entries[idx].key, key, MAX_KEY_LEN - 1);
    dict->entries[idx].key[MAX_KEY_LEN - 1] = '\0';
    dict->entries[idx].value = value;
    dict->entries[idx].is_occupied = true;
    dict->size++;
    return true;
}

bool all_values_same(const Dictionary *dict, bool *result) {
    if (dict == NULL || result == NULL) {
        return false;
    }

    if (dict->size == 0) {
        *result = true;
        return true;
    }

    int first_value = 0;
    bool found_first = false;

    for (size_t i = 0; i < dict->capacity; i++) {
        if (dict->entries[i].is_occupied) {
            if (!found_first) {
                first_value = dict->entries[i].value;
                found_first = true;
            } else if (dict->entries[i].value != first_value) {
                *result = false;
                return true;
            }
        }
    }

    *result = true;
    return true;
}

int main(void) {
    Dictionary dict;
    bool result;

    if (!dict_init(&dict)) {
        fprintf(stderr, "Failed to initialize dictionary\n");
        return EXIT_FAILURE;
    }

    if (!dict_insert(&dict, "a", 5) ||
        !dict_insert(&dict, "b", 5) ||
        !dict_insert(&dict, "c", 5)) {
        fprintf(stderr, "Failed to insert into dictionary\n");
        dict_free(&dict);
        return EXIT_FAILURE;
    }

    if (!all_values_same(&dict, &result)) {
        fprintf(stderr, "Failed to check dictionary values\n");
        dict_free(&dict);
        return EXIT_FAILURE;
    }

    printf("Test 1 (all same): %s\n", result ? "true" : "false");

    if (!dict_insert(&dict, "d", 10)) {
        fprintf(stderr, "Failed to insert into dictionary\n");
        dict_free(&dict);
        return EXIT_FAILURE;
    }

    if (!all_values_same(&dict, &result)) {
        fprintf(stderr, "Failed to check dictionary values\n");
        dict_free(&dict);
        return EXIT_FAILURE;
    }

    printf("Test 2 (different values): %s\n", result ? "true" : "false");

    dict_free(&dict);

    Dictionary empty_dict;
    if (!dict_init(&empty_dict)) {
        fprintf(stderr, "Failed to initialize dictionary\n");
        return EXIT_FAILURE;
    }

    if (!all_values_same(&empty_dict, &result)) {
        fprintf(stderr, "Failed to check dictionary values\n");
        dict_free(&empty_dict);
        return EXIT_FAILURE;
    }

    printf("Test 3 (empty dictionary): %s\n", result ? "true" : "false");

    dict_free(&empty_dict);

    return EXIT_SUCCESS;
}