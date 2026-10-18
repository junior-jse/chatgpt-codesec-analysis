#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_KEY_LENGTH 1024

typedef struct {
    char *key;
    int value;
} Entry;

typedef struct {
    Entry *entries;
    size_t size;
    size_t capacity;
} Dictionary;

int dict_init(Dictionary *dict, size_t capacity) {
    if (dict == NULL) {
        return -1;
    }
    dict->entries = malloc(capacity * sizeof(Entry));
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

int dict_resize(Dictionary *dict) {
    if (dict == NULL) {
        return -1;
    }

    size_t new_capacity = dict->capacity * 2;
    Entry *new_entries = realloc(dict->entries, new_capacity * sizeof(Entry));
    if (new_entries == NULL) {
        return -1;
    }

    dict->entries = new_entries;
    dict->capacity = new_capacity;
    return 0;
}

int dict_add(Dictionary *dict, const char *key, int value) {
    if (dict == NULL || key == NULL) {
        return -1;
    }

    if (dict->size >= dict->capacity) {
        if (dict_resize(dict) != 0) {
            return -1;
        }
    }

    size_t key_len = strnlen(key, MAX_KEY_LENGTH);
    if (key_len == MAX_KEY_LENGTH) {
        return -1;
    }

    char *key_copy = malloc(key_len + 1);
    if (key_copy == NULL) {
        return -1;
    }
    memcpy(key_copy, key, key_len);
    key_copy[key_len] = '\0';

    dict->entries[dict->size].key = key_copy;
    dict->entries[dict->size].value = value;
    dict->size++;

    return 0;
}

int dict_find(const Dictionary *dict, const char *key) {
    if (dict == NULL || key == NULL) {
        return -1;
    }

    for (size_t i = 0; i < dict->size; i++) {
        if (strcmp(dict->entries[i].key, key) == 0) {
            return (int)i;
        }
    }
    return -1;
}

int dict_combine(const Dictionary *dict1, const Dictionary *dict2, Dictionary *result) {
    if (dict1 == NULL || dict2 == NULL || result == NULL) {
        return -1;
    }

    if (dict_init(result, dict1->size + dict2->size) != 0) {
        return -1;
    }

    for (size_t i = 0; i < dict1->size; i++) {
        if (dict_add(result, dict1->entries[i].key, dict1->entries[i].value) != 0) {
            dict_free(result);
            return -1;
        }
    }

    for (size_t i = 0; i < dict2->size; i++) {
        int index = dict_find(result, dict2->entries[i].key);
        if (index >= 0) {
            result->entries[index].value += dict2->entries[i].value;
        } else {
            if (dict_add(result, dict2->entries[i].key, dict2->entries[i].value) != 0) {
                dict_free(result);
                return -1;
            }
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
    Dictionary dict1;
    Dictionary dict2;
    Dictionary result;

    if (dict_init(&dict1, 4) != 0) {
        fprintf(stderr, "Failed to initialize dict1\n");
        return EXIT_FAILURE;
    }

    if (dict_init(&dict2, 4) != 0) {
        fprintf(stderr, "Failed to initialize dict2\n");
        dict_free(&dict1);
        return EXIT_FAILURE;
    }

    if (dict_add(&dict1, "apple", 5) != 0 ||
        dict_add(&dict1, "banana", 3) != 0 ||
        dict_add(&dict1, "cherry", 7) != 0) {
        fprintf(stderr, "Failed to add entries to dict1\n");
        dict_free(&dict1);
        dict_free(&dict2);
        return EXIT_FAILURE;
    }

    if (dict_add(&dict2, "banana", 2) != 0 ||
        dict_add(&dict2, "cherry", 1) != 0 ||
        dict_add(&dict2, "date", 4) != 0) {
        fprintf(stderr, "Failed to add entries to dict2\n");
        dict_free(&dict1);
        dict_free(&dict2);
        return EXIT_FAILURE;
    }

    printf("Dictionary 1: ");
    dict_print(&dict1);

    printf("Dictionary 2: ");
    dict_print(&dict2);

    if (dict_combine(&dict1, &dict2, &result) != 0) {
        fprintf(stderr, "Failed to combine dictionaries\n");
        dict_free(&dict1);
        dict_free(&dict2);
        return EXIT_FAILURE;
    }

    printf("Combined: ");
    dict_print(&result);

    dict_free(&dict1);
    dict_free(&dict2);
    dict_free(&result);

    return EXIT_SUCCESS;
}