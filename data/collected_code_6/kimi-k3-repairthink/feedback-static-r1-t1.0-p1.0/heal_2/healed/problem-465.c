#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define DICT_MAX_STRING_LEN 4096

typedef struct {
    char *key;
    char *value;
} DictItem;

typedef struct {
    DictItem *items;
    size_t size;
    size_t capacity;
} Dictionary;

static size_t bounded_strlen(const char *s, size_t max_len) {
    size_t len = 0;
    if (s == NULL) {
        return 0;
    }
    while (len < max_len && s[len] != '\0') {
        len++;
    }
    return len;
}

static char *bounded_strdup(const char *s) {
    size_t len;
    char *copy;
    if (s == NULL) {
        return NULL;
    }
    len = bounded_strlen(s, DICT_MAX_STRING_LEN);
    if (len >= DICT_MAX_STRING_LEN) {
        return NULL;
    }
    copy = malloc(len + 1);
    if (copy == NULL) {
        return NULL;
    }
    memcpy(copy, s, len + 1);
    return copy;
}

Dictionary *dict_create(size_t initial_capacity) {
    Dictionary *dict = malloc(sizeof(Dictionary));
    if (dict == NULL) {
        return NULL;
    }
    if (initial_capacity == 0) {
        initial_capacity = 1;
    }
    if (initial_capacity > SIZE_MAX / sizeof(DictItem)) {
        free(dict);
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
    char *key_copy;
    char *value_copy;

    if (dict == NULL || key == NULL) {
        return -1;
    }

    key_copy = bounded_strdup(key);
    if (key_copy == NULL) {
        return -1;
    }

    if (value != NULL) {
        value_copy = bounded_strdup(value);
        if (value_copy == NULL) {
            free(key_copy);
            return -1;
        }
    } else {
        value_copy = malloc(1);
        if (value_copy == NULL) {
            free(key_copy);
            return -1;
        }
        value_copy[0] = '\0';
    }

    if (dict->size >= dict->capacity) {
        size_t new_capacity;
        DictItem *new_items;
        if (dict->capacity > SIZE_MAX / 2) {
            free(key_copy);
            free(value_copy);
            return -1;
        }
        new_capacity = dict->capacity * 2;
        if (new_capacity > SIZE_MAX / sizeof(DictItem)) {
            free(key_copy);
            free(value_copy);
            return -1;
        }
        new_items = realloc(dict->items, new_capacity * sizeof(DictItem));
        if (new_items == NULL) {
            free(key_copy);
            free(value_copy);
            return -1;
        }
        dict->items = new_items;
        dict->capacity = new_capacity;
    }

    dict->items[dict->size].key = key_copy;
    dict->items[dict->size].value = value_copy;
    dict->size++;
    return 0;
}

void dict_drop_empty(Dictionary *dict) {
    size_t write_index;
    size_t i;
    if (dict == NULL) {
        return;
    }
    write_index = 0;
    for (i = 0; i < dict->size; i++) {
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
    size_t i;
    if (dict == NULL) {
        return;
    }
    for (i = 0; i < dict->size; i++) {
        free(dict->items[i].key);
        free(dict->items[i].value);
    }
    free(dict->items);
    free(dict);
}

void dict_print(const Dictionary *dict) {
    size_t i;
    if (dict == NULL) {
        return;
    }
    printf("{");
    for (i = 0; i < dict->size; i++) {
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