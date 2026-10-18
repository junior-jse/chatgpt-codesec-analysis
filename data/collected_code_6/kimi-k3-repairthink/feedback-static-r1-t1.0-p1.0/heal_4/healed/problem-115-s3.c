#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define MAX_STRING_LENGTH 4096

typedef struct {
    char *key;
    char *value;
} DictEntry;

typedef struct {
    DictEntry *entries;
    size_t size;
    size_t capacity;
} Dictionary;

typedef struct {
    Dictionary *dicts;
    size_t count;
} DictList;

static char *copy_string(const char *src) {
    if (src == NULL) {
        return NULL;
    }
    size_t src_len = strnlen(src, MAX_STRING_LENGTH);
    if (src_len == MAX_STRING_LENGTH) {
        return NULL;
    }
    size_t len = src_len + 1;
    char *copy = malloc(len);
    if (copy == NULL) {
        return NULL;
    }
    for (size_t i = 0; i < src_len; i++) {
        copy[i] = src[i];
    }
    copy[src_len] = '\0';
    return copy;
}

bool all_dicts_empty(const DictList *list) {
    if (list == NULL || list->dicts == NULL || list->count == 0) {
        return true;
    }
    for (size_t i = 0; i < list->count; i++) {
        if (list->dicts[i].entries != NULL && list->dicts[i].size > 0) {
            return false;
        }
    }
    return true;
}

bool dict_init(Dictionary *dict, size_t capacity) {
    if (dict == NULL || capacity == 0) {
        return false;
    }
    dict->entries = calloc(capacity, sizeof(DictEntry));
    if (dict->entries == NULL) {
        return false;
    }
    dict->size = 0;
    dict->capacity = capacity;
    return true;
}

void dict_free(Dictionary *dict) {
    if (dict != NULL && dict->entries != NULL) {
        for (size_t i = 0; i < dict->size; i++) {
            free(dict->entries[i].key);
            free(dict->entries[i].value);
            dict->entries[i].key = NULL;
            dict->entries[i].value = NULL;
        }
        free(dict->entries);
        dict->entries = NULL;
        dict->size = 0;
        dict->capacity = 0;
    }
}

bool dict_add(Dictionary *dict, const char *key, const char *value) {
    if (dict == NULL || key == NULL || value == NULL || dict->entries == NULL) {
        return false;
    }
    if (dict->size >= dict->capacity) {
        return false;
    }
    char *key_copy = copy_string(key);
    if (key_copy == NULL) {
        return false;
    }
    char *value_copy = copy_string(value);
    if (value_copy == NULL) {
        free(key_copy);
        return false;
    }
    dict->entries[dict->size].key = key_copy;
    dict->entries[dict->size].value = value_copy;
    dict->size++;
    return true;
}

int main(void) {
    DictList list = {0};
    list.count = 3;
    list.dicts = calloc(list.count, sizeof(Dictionary));
    if (list.dicts == NULL) {
        return 1;
    }

    for (size_t i = 0; i < list.count; i++) {
        if (!dict_init(&list.dicts[i], 10)) {
            for (size_t j = 0; j < i; j++) {
                dict_free(&list.dicts[j]);
            }
            free(list.dicts);
            return 1;
        }
    }

    printf("All empty: %s\n", all_dicts_empty(&list) ? "true" : "false");

    if (!dict_add(&list.dicts[1], "key1", "value1")) {
        for (size_t i = 0; i < list.count; i++) {
            dict_free(&list.dicts[i]);
        }
        free(list.dicts);
        return 1;
    }

    printf("All empty: %s\n", all_dicts_empty(&list) ? "true" : "false");

    for (size_t i = 0; i < list.count; i++) {
        dict_free(&list.dicts[i]);
    }
    free(list.dicts);

    return 0;
}