#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_KEY_LEN 64

typedef struct {
    char **keys;
    int *values;
    int size;
} Dictionary;

char** get_dictionary_keys(Dictionary* dict, int* out_size) {
    if (dict == NULL || out_size == NULL) {
        return NULL;
    }

    if (dict->size <= 0) {
        *out_size = 0;
        return NULL;
    }

    char** key_list = malloc(dict->size * sizeof(char*));
    if (key_list == NULL) {
        *out_size = 0;
        return NULL;
    }

    for (int i = 0; i < dict->size; i++) {
        key_list[i] = malloc(MAX_KEY_LEN);
        if (key_list[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(key_list[j]);
            }
            free(key_list);
            *out_size = 0;
            return NULL;
        }
        snprintf(key_list[i], MAX_KEY_LEN, "%s", dict->keys[i]);
    }

    *out_size = dict->size;
    return key_list;
}

void free_key_list(char** key_list, int size) {
    if (key_list == NULL) {
        return;
    }
    for (int i = 0; i < size; i++) {
        free(key_list[i]);
    }
    free(key_list);
}

int main() {
    Dictionary dict;
    dict.size = 3;
    
    dict.keys = malloc(dict.size * sizeof(char*));
    if (dict.keys == NULL) {
        return 1;
    }
    
    dict.values = malloc(dict.size * sizeof(int));
    if (dict.values == NULL) {
        free(dict.keys);
        return 1;
    }
    
    for (int i = 0; i < dict.size; i++) {
        dict.keys[i] = malloc(MAX_KEY_LEN);
        if (dict.keys[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(dict.keys[j]);
            }
            free(dict.keys);
            free(dict.values);
            return 1;
        }
    }
    
    snprintf(dict.keys[0], MAX_KEY_LEN, "%s", "apple");
    dict.values[0] = 1;
    
    snprintf(dict.keys[1], MAX_KEY_LEN, "%s", "banana");
    dict.values[1] = 2;
    
    snprintf(dict.keys[2], MAX_KEY_LEN, "%s", "cherry");
    dict.values[2] = 3;

    int list_size = 0;
    char** keys = get_dictionary_keys(&dict, &list_size);

    if (keys != NULL) {
        for (int i = 0; i < list_size; i++) {
            printf("%s\n", keys[i]);
        }
        free_key_list(keys, list_size);
    }

    for (int i = 0; i < dict.size; i++) {
        free(dict.keys[i]);
    }
    free(dict.keys);
    free(dict.values);

    return 0;
}