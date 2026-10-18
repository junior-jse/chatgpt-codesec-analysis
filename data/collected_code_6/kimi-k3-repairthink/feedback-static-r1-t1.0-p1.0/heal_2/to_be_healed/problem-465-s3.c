#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define DICT_MAX_STR_LEN 65536

typedef struct {
    char *key;
    char *value;
} DictItem;

typedef struct {
    DictItem *items;
    size_t size;
    size_t capacity;
} Dictionary;

static size_t bounded_strlen(const char *s, size_t max_len)
{
    size_t len = 0;

    if (s == NULL) {
        return 0;
    }
    while (len < max_len && s[len] != '\0') {
        len++;
    }
    return len;
}

static char *safe_strdup(const char *s)
{
    size_t len;
    char *copy;

    if (s == NULL) {
        return NULL;
    }
    len = bounded_strlen(s, DICT_MAX_STR_LEN);
    if (len >= DICT_MAX_STR_LEN) {
        return NULL;
    }
    copy = malloc(len + 1);
    if (copy == NULL) {
        return NULL;
    }
    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(copy, s, len + 1);
    return copy;
}

int dict_init(Dictionary *dict, size_t initial_capacity)
{
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

void dict_free(Dictionary *dict)
{
    if (dict == NULL) {
        return;
    }
    if (dict->items != NULL) {
        for (size_t i = 0; i < dict->size; i++) {
            free(dict->items[i].key);
            free(dict->items[i].value);
        }
        free(dict->items);
        dict->items = NULL;
    }
    dict->size = 0;
    dict->capacity = 0;
}

int dict_add(Dictionary *dict, const char *key, const char *value)
{
    char *new_key;
    char *new_value;

    if (dict == NULL || key == NULL) {
        return -1;
    }

    if (dict->size >= dict->capacity) {
        size_t new_capacity;
        DictItem *new_items;

        if (dict->capacity > SIZE_MAX / 2) {
            return -1;
        }
        new_capacity = (dict->capacity == 0) ? 4 : dict->capacity * 2;
        if (new_capacity > SIZE_MAX / sizeof(DictItem)) {
            return -1;
        }
        new_items = realloc(dict->items, new_capacity * sizeof(DictItem));
        if (new_items == NULL) {
            return -1;
        }
        dict->items = new_items;
        dict->capacity = new_capacity;
    }

    new_key = safe_strdup(key);
    if (new_key == NULL) {
        return -1;
    }

    if (value != NULL) {
        new_value = safe_strdup(value);
        if (new_value == NULL) {
            free(new_key);
            return -1;
        }
    } else {
        new_value = NULL;
    }

    dict->items[dict->size].key = new_key;
    dict->items[dict->size].value = new_value;
    dict->size++;
    return 0;
}

int dict_drop_empty(Dictionary *dict)
{
    size_t write_idx = 0;

    if (dict == NULL || dict->items == NULL) {
        return -1;
    }

    for (size_t i = 0; i < dict->size; i++) {
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
        DictItem *new_items = realloc(dict->items, write_idx * sizeof(DictItem));
        if (new_items != NULL) {
            dict->items = new_items;
            dict->capacity = write_idx;
        }
    }

    dict->size = write_idx;
    return 0;
}

void dict_print(const Dictionary *dict)
{
    if (dict == NULL) {
        return;
    }
    for (size_t i = 0; i < dict->size; i++) {
        printf("%s: %s\n", dict->items[i].key,
               dict->items[i].value ? dict->items[i].value : "(null)");
    }
}

int main(void)
{
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