#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *key;
    int value;
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

int dict_contains(const Dictionary *dict, const char *key) {
    if (dict == NULL || key == NULL || dict->entries == NULL) {
        return 0;
    }
    for (size_t i = 0; i < dict->size; i++) {
        if (dict->entries[i].key != NULL && strcmp(dict->entries[i].key, key) == 0) {
            return 1;
        }
    }
    return 0;
}

int dict_add(Dictionary *dict, const char *key, int value) {
    if (dict == NULL || key == NULL || dict->entries == NULL) {
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
    dict->entries[dict->size].key = malloc(strlen(key) + 1);
    if (dict->entries[dict->size].key == NULL) {
        return -1;
    }
    strcpy(dict->entries[dict->size].key, key);
    dict->entries[dict->size].value = value;
    dict->size++;
    return 0;
}

void dict_free(Dictionary *dict) {
    if (dict == NULL || dict->entries == NULL) {
        return;
    }
    for (size_t i = 0; i < dict->size; i++) {
        free(dict->entries[i].key);
        dict->entries[i].key = NULL;
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

    if (dict_add(&dict, "apple", 1) != 0 ||
        dict_add(&dict, "banana", 2) != 0 ||
        dict_add(&dict, "cherry", 3) != 0) {
        fprintf(stderr, "Failed to add entries\n");
        dict_free(&dict);
        return EXIT_FAILURE;
    }

    const char *search_key = "banana";
    if (dict_contains(&dict, search_key)) {
        printf("Key '%s' is present in the dictionary\n", search_key);
    } else {
        printf("Key '%s' is not present in the dictionary\n", search_key);
    }

    search_key = "grape";
    if (dict_contains(&dict, search_key)) {
        printf("Key '%s' is present in the dictionary\n", search_key);
    } else {
        printf("Key '%s' is not present in the dictionary\n", search_key);
    }

    dict_free(&dict);
    return EXIT_SUCCESS;
}