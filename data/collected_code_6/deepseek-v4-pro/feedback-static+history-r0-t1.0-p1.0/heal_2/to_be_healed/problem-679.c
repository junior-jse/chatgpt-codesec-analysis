/* Repair history (previous round):
 * - Flawfinder strcpy was replaced by memcpy, which Flawfinder flags with the same CWE (CWE-120).
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct KeyValue {
    char *key;
    int value;
} KeyValue;

typedef struct Dictionary {
    KeyValue *items;
    int size;
    int capacity;
} Dictionary;

Dictionary *create_dictionary(int capacity) {
    Dictionary *dict = (Dictionary *)malloc(sizeof(Dictionary));
    if (dict == NULL) {
        return NULL;
    }
    dict->items = (KeyValue *)malloc(sizeof(KeyValue) * capacity);
    if (dict->items == NULL) {
        free(dict);
        return NULL;
    }
    dict->size = 0;
    dict->capacity = capacity;
    return dict;
}

int add_key_value(Dictionary *dict, const char *key, int value) {
    if (dict == NULL || key == NULL) {
        return 0;
    }
    if (dict->size >= dict->capacity) {
        return 0;
    }
    for (int i = 0; i < dict->size; i++) {
        if (strcmp(dict->items[i].key, key) == 0) {
            dict->items[i].value = value;
            return 1;
        }
    }
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t key_len = strlen(key);
    dict->items[dict->size].key = (char *)malloc(key_len + 1);
    if (dict->items[dict->size].key == NULL) {
        return 0;
    }
    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(dict->items[dict->size].key, key, key_len + 1);
    dict->items[dict->size].value = value;
    dict->size++;
    return 1;
}

int *get_element_by_index(Dictionary *dict, int index) {
    if (dict == NULL || index < 0 || index >= dict->size) {
        return NULL;
    }
    return &dict->items[index].value;
}

const int *get_element_by_index_const(const Dictionary *dict, int index) {
    if (dict == NULL || index < 0 || index >= dict->size) {
        return NULL;
    }
    return &dict->items[index].value;
}

const char *get_key_by_index(const Dictionary *dict, int index) {
    if (dict == NULL || index < 0 || index >= dict->size) {
        return NULL;
    }
    return dict->items[index].key;
}

void destroy_dictionary(Dictionary *dict) {
    if (dict == NULL) {
        return;
    }
    for (int i = 0; i < dict->size; i++) {
        free(dict->items[i].key);
    }
    free(dict->items);
    free(dict);
}

int main(void) {
    Dictionary *dict = create_dictionary(10);
    if (dict == NULL) {
        return 1;
    }

    add_key_value(dict, "apple", 100);
    add_key_value(dict, "banana", 200);
    add_key_value(dict, "cherry", 300);

    for (int i = 0; i < dict->size; i++) {
        const char *key = get_key_by_index(dict, i);
        const int *value = get_element_by_index_const(dict, i);
        if (key != NULL && value != NULL) {
            printf("%s: %d\n", key, *value);
        }
    }

    const int *value = get_element_by_index_const(dict, 1);
    if (value != NULL) {
        printf("Element at index 1: %d\n", *value);
    }

    destroy_dictionary(dict);
    return 0;
}