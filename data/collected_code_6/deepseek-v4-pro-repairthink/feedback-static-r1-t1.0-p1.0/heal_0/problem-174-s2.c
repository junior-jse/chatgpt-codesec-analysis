#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 16
#define MAX_KEY_LENGTH 128

typedef struct Pair {
    char *key;
    int value;
} Pair;

typedef struct ValueList {
    int *values;
    size_t count;
    size_t capacity;
} ValueList;

typedef struct DictEntry {
    char *key;
    ValueList list;
} DictEntry;

typedef struct Dictionary {
    DictEntry *entries;
    size_t size;
    size_t capacity;
} Dictionary;

static int dict_init(Dictionary *dict) {
    if (!dict) return -1;
    dict->capacity = INITIAL_CAPACITY;
    dict->size = 0;
    dict->entries = (DictEntry *)calloc(dict->capacity, sizeof(DictEntry));
    if (!dict->entries) return -1;
    return 0;
}

static void dict_destroy(Dictionary *dict) {
    if (!dict) return;
    for (size_t i = 0; i < dict->size; i++) {
        free(dict->entries[i].key);
        free(dict->entries[i].list.values);
    }
    free(dict->entries);
    dict->entries = NULL;
    dict->size = 0;
    dict->capacity = 0;
}

static int dict_find_or_create(Dictionary *dict, const char *key, size_t *index_out) {
    if (!dict || !key || !index_out) return -1;

    for (size_t i = 0; i < dict->size; i++) {
        if (strcmp(dict->entries[i].key, key) == 0) {
            *index_out = i;
            return 0;
        }
    }

    if (dict->size >= dict->capacity) {
        size_t new_capacity = dict->capacity * 2;
        DictEntry *new_entries = (DictEntry *)realloc(dict->entries, new_capacity * sizeof(DictEntry));
        if (!new_entries) return -1;
        dict->entries = new_entries;
        for (size_t i = dict->capacity; i < new_capacity; i++) {
            dict->entries[i].key = NULL;
            dict->entries[i].list.values = NULL;
            dict->entries[i].list.count = 0;
            dict->entries[i].list.capacity = 0;
        }
        dict->capacity = new_capacity;
    }

    size_t idx = dict->size;
    dict->entries[idx].key = (char *)malloc(strlen(key) + 1);
    if (!dict->entries[idx].key) return -1;
    strcpy(dict->entries[idx].key, key);
    dict->entries[idx].list.values = NULL;
    dict->entries[idx].list.count = 0;
    dict->entries[idx].list.capacity = 0;
    dict->size++;
    *index_out = idx;
    return 0;
}

static int value_list_append(ValueList *list, int value) {
    if (!list) return -1;
    if (list->count >= list->capacity) {
        size_t new_capacity = list->capacity == 0 ? 4 : list->capacity * 2;
        int *new_values = (int *)realloc(list->values, new_capacity * sizeof(int));
        if (!new_values) return -1;
        list->values = new_values;
        list->capacity = new_capacity;
    }
    list->values[list->count++] = value;
    return 0;
}

Dictionary *group_pairs(Pair *pairs, size_t pair_count) {
    if (!pairs && pair_count > 0) return NULL;

    Dictionary *dict = (Dictionary *)malloc(sizeof(Dictionary));
    if (!dict) return NULL;

    if (dict_init(dict) != 0) {
        free(dict);
        return NULL;
    }

    for (size_t i = 0; i < pair_count; i++) {
        if (!pairs[i].key) {
            dict_destroy(dict);
            free(dict);
            return NULL;
        }

        size_t idx;
        if (dict_find_or_create(dict, pairs[i].key, &idx) != 0) {
            dict_destroy(dict);
            free(dict);
            return NULL;
        }

        if (value_list_append(&dict->entries[idx].list, pairs[i].value) != 0) {
            dict_destroy(dict);
            free(dict);
            return NULL;
        }
    }

    return dict;
}

void print_dictionary(Dictionary *dict) {
    if (!dict) return;
    for (size_t i = 0; i < dict->size; i++) {
        printf("%s: [", dict->entries[i].key);
        for (size_t j = 0; j < dict->entries[i].list.count; j++) {
            printf("%d", dict->entries[i].list.values[j]);
            if (j < dict->entries[i].list.count - 1) printf(", ");
        }
        printf("]\n");
    }
}

int main(void) {
    Pair pairs[] = {
        {"apple", 1},
        {"banana", 2},
        {"apple", 3},
        {"cherry", 4},
        {"banana", 5},
        {"apple", 6},
        {"date", 7}
    };
    size_t pair_count = sizeof(pairs) / sizeof(pairs[0]);

    Dictionary *dict = group_pairs(pairs, pair_count);
    if (!dict) {
        fprintf(stderr, "Failed to group pairs\n");
        return 1;
    }

    print_dictionary(dict);
    dict_destroy(dict);
    free(dict);

    return 0;
}