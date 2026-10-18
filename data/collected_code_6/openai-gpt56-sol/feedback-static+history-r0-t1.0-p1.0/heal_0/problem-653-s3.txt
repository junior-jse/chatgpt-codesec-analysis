#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    const char *key;
    const char *value;
} KeyValuePair;

typedef struct {
    char **items;
    size_t count;
    size_t capacity;
} StringList;

typedef struct {
    char *key;
    StringList values;
} MapEntry;

typedef struct {
    MapEntry *entries;
    size_t count;
    size_t capacity;
} ListMap;

static char *duplicate_string(const char *source)
{
    size_t length;
    char *copy;

    if (source == NULL) {
        return NULL;
    }

    length = strlen(source);
    if (length == SIZE_MAX) {
        return NULL;
    }

    copy = malloc(length + 1U);
    if (copy == NULL) {
        return NULL;
    }

    memcpy(copy, source, length + 1U);
    return copy;
}

static int grow_array(void **array, size_t element_size, size_t *capacity)
{
    size_t new_capacity;
    void *new_array;

    if (array == NULL || capacity == NULL || element_size == 0U) {
        return 0;
    }

    if (*capacity == 0U) {
        new_capacity = 4U;
    } else {
        if (*capacity > SIZE_MAX / 2U) {
            return 0;
        }
        new_capacity = *capacity * 2U;
    }

    if (new_capacity > SIZE_MAX / element_size) {
        return 0;
    }

    new_array = realloc(*array, new_capacity * element_size);
    if (new_array == NULL) {
        return 0;
    }

    *array = new_array;
    *capacity = new_capacity;
    return 1;
}

static void free_string_list(StringList *list)
{
    size_t i;

    if (list == NULL) {
        return;
    }

    for (i = 0U; i < list->count; ++i) {
        free(list->items[i]);
    }

    free(list->items);
    list->items = NULL;
    list->count = 0U;
    list->capacity = 0U;
}

static void free_list_map(ListMap *map)
{
    size_t i;

    if (map == NULL) {
        return;
    }

    for (i = 0U; i < map->count; ++i) {
        free(map->entries[i].key);
        free_string_list(&map->entries[i].values);
    }

    free(map->entries);
    map->entries = NULL;
    map->count = 0U;
    map->capacity = 0U;
}

static MapEntry *find_entry(ListMap *map, const char *key)
{
    size_t i;

    if (map == NULL || key == NULL) {
        return NULL;
    }

    for (i = 0U; i < map->count; ++i) {
        if (strcmp(map->entries[i].key, key) == 0) {
            return &map->entries[i];
        }
    }

    return NULL;
}

static int append_value(StringList *list, const char *value)
{
    char *copy;

    if (list == NULL || value == NULL) {
        return 0;
    }

    if (list->count == list->capacity) {
        if (!grow_array((void **)&list->items, sizeof(*list->items),
                        &list->capacity)) {
            return 0;
        }
    }

    copy = duplicate_string(value);
    if (copy == NULL) {
        return 0;
    }

    list->items[list->count++] = copy;
    return 1;
}

static MapEntry *add_entry(ListMap *map, const char *key)
{
    MapEntry *entry;
    char *key_copy;

    if (map == NULL || key == NULL) {
        return NULL;
    }

    if (map->count == map->capacity) {
        if (!grow_array((void **)&map->entries, sizeof(*map->entries),
                        &map->capacity)) {
            return NULL;
        }
    }

    key_copy = duplicate_string(key);
    if (key_copy == NULL) {
        return NULL;
    }

    entry = &map->entries[map->count];
    entry->key = key_copy;
    entry->values.items = NULL;
    entry->values.count = 0U;
    entry->values.capacity = 0U;
    ++map->count;

    return entry;
}

static int group_pairs(const KeyValuePair *pairs, size_t pair_count,
                       ListMap *result)
{
    size_t i;
    MapEntry *entry;

    if (result == NULL || (pairs == NULL && pair_count != 0U)) {
        return 0;
    }

    result->entries = NULL;
    result->count = 0U;
    result->capacity = 0U;

    for (i = 0U; i < pair_count; ++i) {
        if (pairs[i].key == NULL || pairs[i].value == NULL) {
            free_list_map(result);
            return 0;
        }

        entry = find_entry(result, pairs[i].key);
        if (entry == NULL) {
            entry = add_entry(result, pairs[i].key);
            if (entry == NULL) {
                free_list_map(result);
                return 0;
            }
        }

        if (!append_value(&entry->values, pairs[i].value)) {
            free_list_map(result);
            return 0;
        }
    }

    return 1;
}

static int print_list_map(const ListMap *map)
{
    size_t i;
    size_t j;

    if (map == NULL) {
        return 0;
    }

    for (i = 0U; i < map->count; ++i) {
        if (printf("%s: [", map->entries[i].key) < 0) {
            return 0;
        }

        for (j = 0U; j < map->entries[i].values.count; ++j) {
            if (printf("%s\"%s\"", j == 0U ? "" : ", ",
                       map->entries[i].values.items[j]) < 0) {
                return 0;
            }
        }

        if (printf("]\n") < 0) {
            return 0;
        }
    }

    return 1;
}

int main(void)
{
    const KeyValuePair pairs[] = {
        {"fruit", "apple"},
        {"color", "red"},
        {"fruit", "banana"},
        {"color", "blue"},
        {"fruit", "orange"}
    };
    ListMap map;
    size_t pair_count = sizeof(pairs) / sizeof(pairs[0]);
    int status = EXIT_SUCCESS;

    if (!group_pairs(pairs, pair_count, &map)) {
        if (fprintf(stderr, "Failed to group key-value pairs.\n") < 0) {
            return EXIT_FAILURE;
        }
        return EXIT_FAILURE;
    }

    if (!print_list_map(&map)) {
        status = EXIT_FAILURE;
    }

    free_list_map(&map);

    if (status == EXIT_SUCCESS && fflush(stdout) == EOF) {
        status = EXIT_FAILURE;
    }

    return status;
}