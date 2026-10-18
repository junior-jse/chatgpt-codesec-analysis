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

int dict_init(Dictionary *dict, size_t capacity) {
    if (dict == NULL || capacity == 0) {
        return -1;
    }
    dict->entries = calloc(capacity, sizeof(DictEntry));
    if (dict->entries == NULL) {
        return -1;
    }
    dict->size = 0;
    dict->capacity = capacity;
    return 0;
}

void dict_free(Dictionary *dict) {
    if (dict == NULL) {
        return;
    }
    if (dict->entries != NULL) {
        for (size_t i = 0; i < dict->size; i++) {
            free(dict->entries[i].key);
        }
        free(dict->entries);
        dict->entries = NULL;
    }
    dict->size = 0;
    dict->capacity = 0;
}

int dict_insert(Dictionary *dict, const char *key, int value) {
    if (dict == NULL || key == NULL) {
        return -1;
    }
    for (size_t i = 0; i < dict->size; i++) {
        if (strcmp(dict->entries[i].key, key) == 0) {
            dict->entries[i].value = value;
            return 0;
        }
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
    char *key_copy = malloc(strlen(key) + 1);
    if (key_copy == NULL) {
        return -1;
    }
    strcpy(key_copy, key);
    dict->entries[dict->size].key = key_copy;
    dict->entries[dict->size].value = value;
    dict->size++;
    return 0;
}

int dict_merge(Dictionary *dest, const Dictionary *src) {
    if (dest == NULL || src == NULL) {
        return -1;
    }
    for (size_t i = 0; i < src->size; i++) {
        if (dict_insert(dest, src->entries[i].key, src->entries[i].value) != 0) {
            return -1;
        }
    }
    return 0;
}

void dict_print(const Dictionary *dict) {
    if (dict == NULL) {
        return;
    }
    printf("{");
    for (size_t i = 0; i < dict->size; i++) {
        printf("\"%s\": %d", dict->entries[i].key, dict->entries[i].value);
        if (i < dict->size - 1) {
            printf(", ");
        }
    }
    printf("}\n");
}

int main(void) {
    Dictionary dict1, dict2;

    if (dict_init(&dict1, 4) != 0) {
        fprintf(stderr, "Failed to initialize dict1\n");
        return EXIT_FAILURE;
    }
    if (dict_init(&dict2, 4) != 0) {
        fprintf(stderr, "Failed to initialize dict2\n");
        dict_free(&dict1);
        return EXIT_FAILURE;
    }

    if (dict_insert(&dict1, "apple", 1) != 0 ||
        dict_insert(&dict1, "banana", 2) != 0 ||
        dict_insert(&dict1, "cherry", 3) != 0) {
        fprintf(stderr, "Failed to insert into dict1\n");
        dict_free(&dict1);
        dict_free(&dict2);
        return EXIT_FAILURE;
    }

    if (dict_insert(&dict2, "banana", 20) != 0 ||
        dict_insert(&dict2, "date", 4) != 0 ||
        dict_insert(&dict2, "elderberry", 5) != 0) {
        fprintf(stderr, "Failed to insert into dict2\n");
        dict_free(&dict1);
        dict_free(&dict2);
        return EXIT_FAILURE;
    }

    printf("Dictionary 1: ");
    dict_print(&dict1);
    printf("Dictionary 2: ");
    dict_print(&dict2);

    if (dict_merge(&dict1, &dict2) != 0) {
        fprintf(stderr, "Failed to merge dictionaries\n");
        dict_free(&dict1);
        dict_free(&dict2);
        return EXIT_FAILURE;
    }

    printf("Merged Dictionary: ");
    dict_print(&dict1);

    dict_free(&dict1);
    dict_free(&dict2);

    return EXIT_SUCCESS;
}