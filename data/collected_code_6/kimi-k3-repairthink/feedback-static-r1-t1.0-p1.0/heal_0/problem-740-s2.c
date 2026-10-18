#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char* key;
    char* value;
} KeyValuePair;

typedef struct {
    KeyValuePair* pairs;
    size_t size;
    size_t capacity;
} Dictionary;

Dictionary* dictionary_create(size_t initial_capacity) {
    Dictionary* dict = (Dictionary*)malloc(sizeof(Dictionary));
    if (dict == NULL) {
        return NULL;
    }
    
    dict->pairs = (KeyValuePair*)calloc(initial_capacity, sizeof(KeyValuePair));
    if (dict->pairs == NULL) {
        free(dict);
        return NULL;
    }
    
    dict->size = 0;
    dict->capacity = initial_capacity;
    return dict;
}

int dictionary_resize(Dictionary* dict) {
    size_t new_capacity = dict->capacity * 2;
    KeyValuePair* new_pairs = (KeyValuePair*)realloc(dict->pairs, new_capacity * sizeof(KeyValuePair));
    if (new_pairs == NULL) {
        return 0;
    }
    
    dict->pairs = new_pairs;
    dict->capacity = new_capacity;
    return 1;
}

int dictionary_add(Dictionary* dict, const char* key, const char* value) {
    if (dict == NULL || key == NULL || value == NULL) {
        return 0;
    }
    
    if (dict->size >= dict->capacity) {
        if (!dictionary_resize(dict)) {
            return 0;
        }
    }
    
    size_t key_len = strlen(key) + 1;
    size_t value_len = strlen(value) + 1;
    
    dict->pairs[dict->size].key = (char*)malloc(key_len);
    if (dict->pairs[dict->size].key == NULL) {
        return 0;
    }
    
    dict->pairs[dict->size].value = (char*)malloc(value_len);
    if (dict->pairs[dict->size].value == NULL) {
        free(dict->pairs[dict->size].key);
        return 0;
    }
    
    strncpy(dict->pairs[dict->size].key, key, key_len - 1);
    dict->pairs[dict->size].key[key_len - 1] = '\0';
    
    strncpy(dict->pairs[dict->size].value, value, value_len - 1);
    dict->pairs[dict->size].value[value_len - 1] = '\0';
    
    dict->size++;
    return 1;
}

Dictionary* tuple_to_dictionary(char** tuple, size_t tuple_size) {
    if (tuple == NULL || tuple_size == 0) {
        return NULL;
    }
    
    size_t pair_count = tuple_size / 2;
    Dictionary* dict = dictionary_create(pair_count);
    if (dict == NULL) {
        return NULL;
    }
    
    for (size_t i = 0; i < tuple_size - 1; i += 2) {
        if (tuple[i] == NULL || tuple[i + 1] == NULL) {
            continue;
        }
        
        if (!dictionary_add(dict, tuple[i], tuple[i + 1])) {
            for (size_t j = 0; j < dict->size; j++) {
                free(dict->pairs[j].key);
                free(dict->pairs[j].value);
            }
            free(dict->pairs);
            free(dict);
            return NULL;
        }
    }
    
    return dict;
}

void dictionary_free(Dictionary* dict) {
    if (dict == NULL) {
        return;
    }
    
    for (size_t i = 0; i < dict->size; i++) {
        free(dict->pairs[i].key);
        free(dict->pairs[i].value);
    }
    free(dict->pairs);
    free(dict);
}

void dictionary_print(const Dictionary* dict) {
    if (dict == NULL) {
        return;
    }
    
    printf("{");
    for (size_t i = 0; i < dict->size; i++) {
        printf("'%s': '%s'", dict->pairs[i].key, dict->pairs[i].value);
        if (i < dict->size - 1) {
            printf(", ");
        }
    }
    printf("}\n");
}

int main(void) {
    char* tuple[] = {"name", "John", "age", "30", "city", "New York"};
    size_t tuple_size = sizeof(tuple) / sizeof(tuple[0]);
    
    Dictionary* dict = tuple_to_dictionary(tuple, tuple_size);
    if (dict == NULL) {
        fprintf(stderr, "Failed to create dictionary\n");
        return EXIT_FAILURE;
    }
    
    dictionary_print(dict);
    dictionary_free(dict);
    
    return EXIT_SUCCESS;
}