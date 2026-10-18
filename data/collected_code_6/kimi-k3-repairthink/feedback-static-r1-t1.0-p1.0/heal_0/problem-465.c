#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *key;
    char *value;
} DictItem;

typedef struct {
    DictItem *items;
    size_t size;
    size_t capacity;
} Dictionary;

Dictionary *dict_create(size_t initial_capacity) {
    Dictionary *dict = malloc(sizeof(Dictionary));
    if (dict == NULL) {
        return NULL;
    }
    dict->items = malloc(initial_capacity * sizeof(DictItem));
    if (dict->items == NULL) {
        free(dict);
        return NULL;
    }
    dict->size = 0;
    dict->capacity = initial_capacity;
    return dict;
}

int dict_add(Dictionary *dict, const char *key, const char *value) {
    if (dict == NULL || key == NULL) {
        return -1;
    }
    if (dict->size >= dict->capacity) {
        size_t new_capacity = dict->capacity * 2;
        DictItem *new_items = realloc(dict->items, new_capacity * sizeof(DictItem));
        if (new_items == NULL) {
            return -1;
        }
        dict->items = new_items;
        dict->capacity = new_capacity;
    }
    dict->items[dict->size].key = malloc(strlen(key) + 1);
    if (dict->items[dict->size].key == NULL) {
        return -1;
    }
    strcpy(dict->items[dict->size].key, key);
    if (value != NULL) {
        dict->items[dict->size].value = malloc(strlen(value) + 1);
        if (dict->items[dict->size].value == NULL) {
            free(dict->items[dict->size].key);
            return -1;
        }
        strcpy(dict->items[dict->size].value, value);
    } else {
        dict->items[dict->size].value = malloc(1);
        if (dict->items[dict->size].value == NULL) {
            free(dict->items[dict->size].key);
            return -1;
        }
        dict->items[dict->size].value[0] = '\0';
    }
    dict->size++;
    return 0;
}

void dict_drop_empty(Dictionary *dict) {
    if (dict == NULL) {
        return;
    }
    size_t write_index = 0;
    for (size_t i = 0; i < dict->size; i++) {
        if (dict->items[i].value != NULL && dict->items[i].value[0] != '\0') {
            if (write_index != i) {
                dict->items[write_index] = dict->items[i];
            }
            write_index++;
        } else {
            free(dict->items[i].key);
            free(dict->items[i].value);
        }
    }
    dict->size = write_index;
}

void dict_free(Dictionary *dict) {
    if (dict == NULL) {
        return;
    }
    for (size_t i = 0; i < dict->size; i++) {
        free(dict->items[i].key);
        free(dict->items[i].value);
    }
    free(dict->items);
    free(dict);
}

void dict_print(const Dictionary *dict) {
    if (dict == NULL) {
        return;
    }
    printf("{");
    for (size_t i = 0; i < dict->size; i++) {
        printf("\"%s\": \"%s\"", dict->items[i].key, dict->items[i].value);
        if (i < dict->size - 1) {
            printf(", ");
        }
    }
    printf("}\n");
}

int main(void) {
    Dictionary *dict = dict_create(4);
    if (dict == NULL) {
        fprintf(stderr, "Failed to create dictionary\n");
        return EXIT_FAILURE;
    }

    if (dict_add(dict, "name", "John") != 0 ||
        dict_add(dict, "email", "") != 0 ||
        dict_add(dict, "city", "New York") != 0 ||
        dict_add(dict, "phone", "") != 0 ||
        dict_add(dict, "country", "USA") != 0) {
        fprintf(stderr, "Failed to add items\n");
        dict_free(dict);
        return EXIT_FAILURE;
    }

    printf("Before: ");
    dict_print(dict);

    dict_drop_empty(dict);

    printf("After: ");
    dict_print(dict);

    dict_free(dict);
    return EXIT_SUCCESS;
}