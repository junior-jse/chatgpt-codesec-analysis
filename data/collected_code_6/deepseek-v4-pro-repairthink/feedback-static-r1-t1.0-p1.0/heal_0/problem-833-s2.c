#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 8

typedef struct {
    char **data;
    size_t count;
    size_t capacity;
} KeyList;

typedef struct {
    char *key;
    int value;
} DictEntry;

typedef struct {
    DictEntry *entries;
    size_t count;
    size_t capacity;
} Dictionary;

static int keylist_reserve(KeyList *list, size_t new_capacity) {
    char **new_data;
    if (new_capacity <= list->capacity) {
        return 0;
    }
    new_data = realloc(list->data, new_capacity * sizeof(char *));
    if (new_data == NULL) {
        return -1;
    }
    list->data = new_data;
    list->capacity = new_capacity;
    return 0;
}

KeyList *keylist_create(void) {
    KeyList *list = malloc(sizeof(KeyList));
    if (list == NULL) {
        return NULL;
    }
    list->data = malloc(INITIAL_CAPACITY * sizeof(char *));
    if (list->data == NULL) {
        free(list);
        return NULL;
    }
    list->count = 0;
    list->capacity = INITIAL_CAPACITY;
    return list;
}

void keylist_destroy(KeyList *list) {
    size_t i;
    if (list == NULL) {
        return;
    }
    for (i = 0; i < list->count; i++) {
        free(list->data[i]);
    }
    free(list->data);
    free(list);
}

int keylist_append(KeyList *list, const char *key) {
    char *copy;
    if (list == NULL || key == NULL) {
        return -1;
    }
    if (list->count == list->capacity) {
        if (keylist_reserve(list, list->capacity * 2) != 0) {
            return -1;
        }
    }
    copy = malloc(strlen(key) + 1);
    if (copy == NULL) {
        return -1;
    }
    strcpy(copy, key);
    list->data[list->count] = copy;
    list->count++;
    return 0;
}

Dictionary *dict_create(size_t initial_capacity) {
    Dictionary *dict = malloc(sizeof(Dictionary));
    if (dict == NULL) {
        return NULL;
    }
    if (initial_capacity == 0) {
        initial_capacity = INITIAL_CAPACITY;
    }
    dict->entries = calloc(initial_capacity, sizeof(DictEntry));
    if (dict->entries == NULL) {
        free(dict);
        return NULL;
    }
    dict->count = 0;
    dict->capacity = initial_capacity;
    return dict;
}

void dict_destroy(Dictionary *dict) {
    size_t i;
    if (dict == NULL) {
        return;
    }
    for (i = 0; i < dict->count; i++) {
        free(dict->entries[i].key);
    }
    free(dict->entries);
    free(dict);
}

int dict_set(Dictionary *dict, const char *key, int value) {
    size_t i;
    char *copy;
    DictEntry *new_entries;
    if (dict == NULL || key == NULL) {
        return -1;
    }
    for (i = 0; i < dict->count; i++) {
        if (strcmp(dict->entries[i].key, key) == 0) {
            dict->entries[i].value = value;
            return 0;
        }
    }
    if (dict->count == dict->capacity) {
        new_entries = realloc(dict->entries, dict->capacity * 2 * sizeof(DictEntry));
        if (new_entries == NULL) {
            return -1;
        }
        dict->entries = new_entries;
        dict->capacity *= 2;
    }
    copy = malloc(strlen(key) + 1);
    if (copy == NULL) {
        return -1;
    }
    strcpy(copy, key);
    dict->entries[dict->count].key = copy;
    dict->entries[dict->count].value = value;
    dict->count++;
    return 0;
}

KeyList *dict_keys(const Dictionary *dict) {
    KeyList *keys;
    size_t i;
    if (dict == NULL) {
        return NULL;
    }
    keys = keylist_create();
    if (keys == NULL) {
        return NULL;
    }
    for (i = 0; i < dict->count; i++) {
        if (keylist_append(keys, dict->entries[i].key) != 0) {
            keylist_destroy(keys);
            return NULL;
        }
    }
    return keys;
}

int main(void) {
    Dictionary *dict;
    KeyList *keys;
    size_t i;

    dict = dict_create(4);
    if (dict == NULL) {
        fprintf(stderr, "Failed to create dictionary\n");
        return EXIT_FAILURE;
    }

    if (dict_set(dict, "apple", 1) != 0 ||
        dict_set(dict, "banana", 2) != 0 ||
        dict_set(dict, "cherry", 3) != 0 ||
        dict_set(dict, "date", 4) != 0) {
        fprintf(stderr, "Failed to set dictionary entries\n");
        dict_destroy(dict);
        return EXIT_FAILURE;
    }

    keys = dict_keys(dict);
    if (keys == NULL) {
        fprintf(stderr, "Failed to get dictionary keys\n");
        dict_destroy(dict);
        return EXIT_FAILURE;
    }

    printf("Dictionary keys (%zu):\n", keys->count);
    for (i = 0; i < keys->count; i++) {
        printf("  [%zu] %s\n", i, keys->data[i]);
    }

    keylist_destroy(keys);
    dict_destroy(dict);
    return EXIT_SUCCESS;
}