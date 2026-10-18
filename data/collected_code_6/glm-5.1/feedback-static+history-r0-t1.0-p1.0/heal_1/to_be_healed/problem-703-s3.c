#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char **keys;
    char **values;
    size_t size;
    size_t capacity;
} Dictionary;

Dictionary *dictionary_create(size_t capacity) {
    Dictionary *dict = (Dictionary *)malloc(sizeof(Dictionary));
    if (!dict) {
        return NULL;
    }
    dict->keys = (char **)malloc(capacity * sizeof(char *));
    if (!dict->keys) {
        free(dict);
        return NULL;
    }
    dict->values = (char **)malloc(capacity * sizeof(char *));
    if (!dict->values) {
        free(dict->keys);
        free(dict);
        return NULL;
    }
    dict->size = 0;
    dict->capacity = capacity;
    return dict;
}

int dictionary_insert(Dictionary *dict, const char *key, const char *value) {
    if (!dict || !key || !value || dict->size >= dict->capacity) {
        return 0;
    }
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t key_len = strlen(key) + 1;
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t value_len = strlen(value) + 1;
    dict->keys[dict->size] = (char *)malloc(key_len);
    if (!dict->keys[dict->size]) {
        return 0;
    }
    dict->values[dict->size] = (char *)malloc(value_len);
    if (!dict->values[dict->size]) {
        free(dict->keys[dict->size]);
        return 0;
    }
    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(dict->keys[dict->size], key, key_len);
    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(dict->values[dict->size], value, value_len);
    dict->size++;
    return 1;
}

int dictionary_contains(Dictionary *dict, const char *key) {
    if (!dict || !key) {
        return 0;
    }
    for (size_t i = 0; i < dict->size; i++) {
        if (strcmp(dict->keys[i], key) == 0) {
            return 1;
        }
    }
    return 0;
}

void dictionary_destroy(Dictionary *dict) {
    if (!dict) {
        return;
    }
    for (size_t i = 0; i < dict->size; i++) {
        free(dict->keys[i]);
        free(dict->values[i]);
    }
    free(dict->keys);
    free(dict->values);
    free(dict);
}

int main(void) {
    Dictionary *dict = dictionary_create(10);
    if (!dict) {
        return 1;
    }
    if (!dictionary_insert(dict, "apple", "fruit")) {
        dictionary_destroy(dict);
        return 1;
    }
    if (!dictionary_insert(dict, "broccoli", "vegetable")) {
        dictionary_destroy(dict);
        return 1;
    }
    if (!dictionary_insert(dict, "salmon", "fish")) {
        dictionary_destroy(dict);
        return 1;
    }
    if (dictionary_contains(dict, "apple")) {
        printf("Key 'apple' found\n");
    } else {
        printf("Key 'apple' not found\n");
    }
    if (dictionary_contains(dict, "beef")) {
        printf("Key 'beef' found\n");
    } else {
        printf("Key 'beef' not found\n");
    }
    dictionary_destroy(dict);
    return 0;
}