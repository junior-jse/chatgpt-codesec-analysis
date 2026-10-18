#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *key;
    char *value;
} KeyValuePair;

typedef struct {
    KeyValuePair *pairs;
    size_t size;
} Dictionary;

Dictionary *create_dictionary(size_t capacity) {
    Dictionary *dict = (Dictionary *)malloc(sizeof(Dictionary));
    if (!dict) return NULL;
    dict->pairs = (KeyValuePair *)calloc(capacity, sizeof(KeyValuePair));
    if (!dict->pairs) {
        free(dict);
        return NULL;
    }
    dict->size = 0;
    return dict;
}

void free_dictionary(Dictionary *dict) {
    if (!dict) return;
    for (size_t i = 0; i < dict->size; i++) {
        free(dict->pairs[i].key);
        free(dict->pairs[i].value);
    }
    free(dict->pairs);
    free(dict);
}

Dictionary *tuple_to_dict(const char **tuple, size_t tuple_size) {
    size_t capacity = tuple_size / 2;
    if (capacity == 0) return NULL;
    
    Dictionary *dict = create_dictionary(capacity);
    if (!dict) return NULL;

    for (size_t i = 0; i < capacity; i++) {
        const char *key = tuple[i * 2];
        const char *value = tuple[i * 2 + 1];
        
        dict->pairs[i].key = strdup(key);
        if (!dict->pairs[i].key) {
            free_dictionary(dict);
            return NULL;
        }

        dict->pairs[i].value = strdup(value);
        if (!dict->pairs[i].value) {
            free(dict->pairs[i].key);
            dict->pairs[i].key = NULL;
            free_dictionary(dict);
            return NULL;
        }
        
        dict->size++;
    }

    return dict;
}

int main(void) {
    const char *my_tuple[] = {"a", "1", "b", "2", "c", "3"};
    size_t tuple_size = sizeof(my_tuple) / sizeof(my_tuple[0]);

    Dictionary *dict = tuple_to_dict(my_tuple, tuple_size);
    if (!dict) {
        fprintf(stderr, "Failed to create dictionary\n");
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < dict->size; i++) {
        printf("Key: %s, Value: %s\n", dict->pairs[i].key, dict->pairs[i].value);
    }

    free_dictionary(dict);
    return EXIT_SUCCESS;
}