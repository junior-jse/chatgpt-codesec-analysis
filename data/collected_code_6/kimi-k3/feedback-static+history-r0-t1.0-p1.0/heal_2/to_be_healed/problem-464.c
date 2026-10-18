#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char key[50];
    int value;
} DictEntry;

typedef struct {
    DictEntry *entries;
    size_t size;
    size_t capacity;
} Dictionary;

bool dict_init(Dictionary *dict, size_t capacity) {
    if (dict == NULL || capacity == 0) {
        return false;
    }
    dict->entries = malloc(capacity * sizeof(DictEntry));
    if (dict->entries == NULL) {
        return false;
    }
    dict->size = 0;
    dict->capacity = capacity;
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

bool dict_add(Dictionary *dict, const char *key, int value) {
    if (dict == NULL || key == NULL || dict->size >= dict->capacity) {
        return false;
    }
    size_t key_len = strnlen(key, sizeof(dict->entries[0].key));
    if (key_len >= sizeof(dict->entries[0].key)) {
        return false;
    }
    snprintf(dict->entries[dict->size].key, sizeof(dict->entries[0].key), "%s", key);
    dict->entries[dict->size].value = value;
    dict->size++;
    return true;
}

bool all_values_same(const Dictionary *dict, bool *result) {
    if (dict == NULL || result == NULL || dict->size == 0) {
        return false;
    }
    *result = true;
    int first_value = dict->entries[0].value;
    for (size_t i = 1; i < dict->size; i++) {
        if (dict->entries[i].value != first_value) {
            *result = false;
            break;
        }
    }
    return true;
}

int main(void) {
    Dictionary dict;
    bool result;
    
    if (!dict_init(&dict, 10)) {
        fprintf(stderr, "Failed to initialize dictionary\n");
        return EXIT_FAILURE;
    }
    
    if (!dict_add(&dict, "a", 5) ||
        !dict_add(&dict, "b", 5) ||
        !dict_add(&dict, "c", 5)) {
        fprintf(stderr, "Failed to add entries\n");
        dict_free(&dict);
        return EXIT_FAILURE;
    }
    
    if (all_values_same(&dict, &result)) {
        printf("All values are %s\n", result ? "same" : "different");
    } else {
        fprintf(stderr, "Error checking values\n");
        dict_free(&dict);
        return EXIT_FAILURE;
    }
    
    dict_free(&dict);
    return EXIT_SUCCESS;
}