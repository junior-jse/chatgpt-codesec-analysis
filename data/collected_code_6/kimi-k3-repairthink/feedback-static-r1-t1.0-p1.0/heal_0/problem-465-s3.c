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

int dict_init(Dictionary *dict, size_t initial_capacity) {
    if (dict == NULL || initial_capacity == 0) {
        return -1;
    }
    dict->items = malloc(initial_capacity * sizeof(DictItem));
    if (dict->items == NULL) {
        return -1;
    }
    dict->size = 0;
    dict->capacity = initial_capacity;
    return 0;
}

void dict_free(Dictionary *dict) {
    size_t i;
    if (dict == NULL) {
        return;
    }
    if (dict->items != NULL) {
        for (i = 0; i < dict->size; i++) {
            free(dict->items[i].key);
            free(dict->items[i].value);
        }
        free(dict->items);
        dict->items = NULL;
    }
    dict->size = 0;
    dict->capacity = 0;
}

int dict_add(Dictionary *dict, const char *key, const char *value) {
    DictItem *new_items;
    char *new_key;
    char *new_value;
    size_t new_capacity;

    if (dict == NULL || key == NULL) {
        return -1;
    }

    if (dict->size >= dict->capacity) {
        new_capacity = dict->capacity * 2;
        new_items = realloc(dict->items, new_capacity * sizeof(DictItem));
        if (new_items == NULL) {
            return -1;
        }
        dict->items = new_items;
        dict->capacity = new_capacity;
    }

    new_key = malloc(strlen(key) + 1);
    if (new_key == NULL) {
        return -1;
    }
    strcpy(new_key, key);

    if (value != NULL) {
        new_value = malloc(strlen(value) + 1);
        if (new_value == NULL) {
            free(new_key);
            return -1;
        }
        strcpy(new_value, value);
    } else {
        new_value = NULL;
    }

    dict->items[dict->size].key = new_key;
    dict->items[dict->size].value = new_value;
    dict->size++;
    return 0;
}

int dict_drop_empty(Dictionary *dict) {
    size_t i;
    size_t write_idx;
    DictItem *new_items;

    if (dict == NULL || dict->items == NULL) {
        return -1;
    }

    write_idx = 0;
    for (i = 0; i < dict->size; i++) {
        if (dict->items[i].value != NULL && dict->items[i].value[0] != '\0') {
            if (write_idx != i) {
                dict->items[write_idx] = dict->items[i];
            }
            write_idx++;
        } else {
            free(dict->items[i].key);
            free(dict->items[i].value);
        }
    }

    if (write_idx == 0) {
        free(dict->items);
        dict->items = NULL;
        dict->size = 0;
        dict->capacity = 0;
        return 0;
    }

    if (write_idx < dict->size) {
        new_items = realloc(dict->items, write_idx * sizeof(DictItem));
        if (new_items != NULL) {
            dict->items = new_items;
            dict->capacity = write_idx;
        }
    }

    dict->size = write_idx;
    return 0;
}

void dict_print(const Dictionary *dict) {
    size_t i;
    if (dict == NULL) {
        return;
    }
    for (i = 0; i < dict->size; i++) {
        printf("%s: %s\n", dict->items[i].key, 
               dict->items[i].value ? dict->items[i].value : "(null)");
    }
}

int main(void) {
    Dictionary dict;
    int result;

    result = dict_init(&dict, 4);
    if (result != 0) {
        fprintf(stderr, "Failed to initialize dictionary\n");
        return EXIT_FAILURE;
    }

    if (dict_add(&dict, "name", "John") != 0 ||
        dict_add(&dict, "empty1", "") != 0 ||
        dict_add(&dict, "age", "30") != 0 ||
        dict_add(&dict, "null_val", NULL) != 0 ||
        dict_add(&dict, "city", "NYC") != 0 ||
        dict_add(&dict, "empty2", "") != 0) {
        fprintf(stderr, "Failed to add items\n");
        dict_free(&dict);
        return EXIT_FAILURE;
    }

    printf("Before dropping empty items:\n");
    dict_print(&dict);

    if (dict_drop_empty(&dict) != 0) {
        fprintf(stderr, "Failed to drop empty items\n");
        dict_free(&dict);
        return EXIT_FAILURE;
    }

    printf("\nAfter dropping empty items:\n");
    dict_print(&dict);

    dict_free(&dict);
    return EXIT_SUCCESS;
}