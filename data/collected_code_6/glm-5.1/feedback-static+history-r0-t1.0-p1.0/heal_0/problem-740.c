#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char key[64];
    char value[64];
} KeyValuePair;

typedef struct {
    KeyValuePair *pairs;
    size_t size;
} Dictionary;

Dictionary* tuple_to_dict(const char **tuple, size_t tuple_size) {
    if (tuple == NULL || tuple_size == 0 || tuple_size % 2 != 0) {
        return NULL;
    }

    Dictionary *dict = (Dictionary*)malloc(sizeof(Dictionary));
    if (dict == NULL) {
        return NULL;
    }

    dict->size = tuple_size / 2;
    dict->pairs = (KeyValuePair*)malloc(dict->size * sizeof(KeyValuePair));
    if (dict->pairs == NULL) {
        free(dict);
        return NULL;
    }

    for (size_t i = 0; i < dict->size; i++) {
        if (tuple[i * 2] == NULL || tuple[i * 2 + 1] == NULL) {
            free(dict->pairs);
            free(dict);
            return NULL;
        }
        strncpy(dict->pairs[i].key, tuple[i * 2], sizeof(dict->pairs[i].key) - 1);
        dict->pairs[i].key[sizeof(dict->pairs[i].key) - 1] = '\0';
        strncpy(dict->pairs[i].value, tuple[i * 2 + 1], sizeof(dict->pairs[i].value) - 1);
        dict->pairs[i].value[sizeof(dict->pairs[i].value) - 1] = '\0';
    }

    return dict;
}

void free_dict(Dictionary *dict) {
    if (dict != NULL) {
        free(dict->pairs);
        free(dict);
    }
}

int main() {
    const char *tuple[] = {"username", "admin", "role", "superuser", "status", "active"};
    size_t tuple_size = sizeof(tuple) / sizeof(tuple[0]);

    Dictionary *dict = tuple_to_dict(tuple, tuple_size);
    if (dict == NULL) {
        fprintf(stderr, "Failed to create dictionary\n");
        return 1;
    }

    for (size_t i = 0; i < dict->size; i++) {
        printf("%s: %s\n", dict->pairs[i].key, dict->pairs[i].value);
    }

    free_dict(dict);
    return 0;
}