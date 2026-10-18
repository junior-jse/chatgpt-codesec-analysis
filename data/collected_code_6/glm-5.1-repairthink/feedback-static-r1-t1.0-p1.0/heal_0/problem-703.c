#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char **keys;
    char **values;
    size_t size;
    size_t capacity;
} Dictionary;

Dictionary *dict_create(size_t capacity) {
    Dictionary *dict = malloc(sizeof(Dictionary));
    if (!dict) {
        return NULL;
    }
    dict->keys = malloc(capacity * sizeof(char *));
    dict->values = malloc(capacity * sizeof(char *));
    if (!dict->keys || !dict->values) {
        free(dict->keys);
        free(dict->values);
        free(dict);
        return NULL;
    }
    dict->size = 0;
    dict->capacity = capacity;
    return dict;
}

void dict_destroy(Dictionary *dict) {
    if (dict) {
        for (size_t i = 0; i < dict->size; i++) {
            free(dict->keys[i]);
            free(dict->values[i]);
        }
        free(dict->keys);
        free(dict->values);
        free(dict);
    }
}

int dict_insert(Dictionary *dict, const char *key, const char *value) {
    if (!dict || !key || !value || dict->size >= dict->capacity) {
        return 0;
    }
    dict->keys[dict->size] = strdup(key);
    dict->values[dict->size] = strdup(value);
    if (!dict->keys[dict->size] || !dict->values[dict->size]) {
        free(dict->keys[dict->size]);
        free(dict->values[dict->size]);
        return 0;
    }
    dict->size++;
    return 1;
}

int dict_contains(Dictionary *dict, const char *key) {
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

int main(void) {
    Dictionary *dict = dict_create(10);
    if (!dict) {
        return EXIT_FAILURE;
    }

    if (!dict_insert(dict, "apple", "fruit")) {
        dict_destroy(dict);
        return EXIT_FAILURE;
    }
    if (!dict_insert(dict, "carrot", "vegetable")) {
        dict_destroy(dict);
        return EXIT_FAILURE;
    }

    const char *search_key = "apple";
    if (dict_contains(dict, search_key)) {
        printf("%s found\n", search_key);
    } else {
        printf("%s not found\n", search_key);
    }

    search_key = "banana";
    if (dict_contains(dict, search_key)) {
        printf("%s found\n", search_key);
    } else {
        printf("%s not found\n", search_key);
    }

    if (dict_contains(dict, NULL)) {
        printf("NULL found\n");
    }

    dict_destroy(dict);
    return EXIT_SUCCESS;
}