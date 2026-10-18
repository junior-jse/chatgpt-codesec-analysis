#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_DICT_SIZE 100
#define MAX_KEY_LENGTH 50

typedef struct {
    char *key;
    int value;
} DictEntry;

typedef struct {
    DictEntry entries[MAX_DICT_SIZE];
    size_t size;
} Dictionary;

int dict_init(Dictionary *dict) {
    if (dict == NULL) {
        return -1;
    }
    dict->size = 0;
    memset(dict->entries, 0, sizeof(dict->entries));
    return 0;
}

int dict_add(Dictionary *dict, const char *key, int value) {
    if (dict == NULL || key == NULL) {
        return -1;
    }
    
    if (dict->size >= MAX_DICT_SIZE) {
        return -1;
    }
    
    size_t key_len = strnlen(key, MAX_KEY_LENGTH);
    if (key_len >= MAX_KEY_LENGTH) {
        return -1;
    }
    
    char *new_key = malloc(key_len + 1);
    if (new_key == NULL) {
        return -1;
    }
    
    int written = snprintf(new_key, key_len + 1, "%s", key);
    if (written < 0 || (size_t)written > key_len) {
        free(new_key);
        return -1;
    }
    
    dict->entries[dict->size].key = new_key;
    dict->entries[dict->size].value = value;
    dict->size++;
    
    return 0;
}

int dict_all_values_same(const Dictionary *dict, int *result) {
    if (dict == NULL || result == NULL) {
        return -1;
    }
    
    if (dict->size == 0) {
        *result = 1;
        return 0;
    }
    
    int first_value = dict->entries[0].value;
    
    for (size_t i = 1; i < dict->size; i++) {
        if (dict->entries[i].value != first_value) {
            *result = 0;
            return 0;
        }
    }
    
    *result = 1;
    return 0;
}

void dict_free(Dictionary *dict) {
    if (dict == NULL) {
        return;
    }
    
    for (size_t i = 0; i < dict->size; i++) {
        free(dict->entries[i].key);
        dict->entries[i].key = NULL;
    }
    dict->size = 0;
}

int main(void) {
    Dictionary dict;
    int result;
    
    if (dict_init(&dict) != 0) {
        fprintf(stderr, "Failed to initialize dictionary\n");
        return EXIT_FAILURE;
    }
    
    if (dict_add(&dict, "apple", 5) != 0) {
        fprintf(stderr, "Failed to add entry\n");
        dict_free(&dict);
        return EXIT_FAILURE;
    }
    
    if (dict_add(&dict, "banana", 5) != 0) {
        fprintf(stderr, "Failed to add entry\n");
        dict_free(&dict);
        return EXIT_FAILURE;
    }
    
    if (dict_add(&dict, "cherry", 5) != 0) {
        fprintf(stderr, "Failed to add entry\n");
        dict_free(&dict);
        return EXIT_FAILURE;
    }
    
    if (dict_all_values_same(&dict, &result) != 0) {
        fprintf(stderr, "Failed to check dictionary values\n");
        dict_free(&dict);
        return EXIT_FAILURE;
    }
    
    if (result) {
        printf("All values are the same\n");
    } else {
        printf("Values are different\n");
    }
    
    if (dict_add(&dict, "date", 10) != 0) {
        fprintf(stderr, "Failed to add entry\n");
        dict_free(&dict);
        return EXIT_FAILURE;
    }
    
    if (dict_all_values_same(&dict, &result) != 0) {
        fprintf(stderr, "Failed to check dictionary values\n");
        dict_free(&dict);
        return EXIT_FAILURE;
    }
    
    if (result) {
        printf("All values are the same\n");
    } else {
        printf("Values are different\n");
    }
    
    dict_free(&dict);
    return EXIT_SUCCESS;
}