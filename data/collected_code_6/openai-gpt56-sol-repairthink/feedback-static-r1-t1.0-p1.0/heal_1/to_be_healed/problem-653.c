#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    const char *key;
    const char *value;
} KeyValuePair;

typedef struct {
    char *key;
    char **values;
    size_t value_count;
    size_t value_capacity;
} MapEntry;

typedef struct {
    MapEntry *entries;
    size_t count;
    size_t capacity;
} Map;

static char *duplicate_string(const char *source)
{
    size_t length;
    char *copy;

    if (source == NULL) {
        return NULL;
    }

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    length = strlen(source);
    if (length == SIZE_MAX) {
        return NULL;
    }

    copy = malloc(length + 1U);
    if (copy == NULL) {
        return NULL;
    }

    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(copy, source, length + 1U);
    return copy;
}

static void map_destroy(Map *map)
{
    size_t i;
    size_t j;

    if (map == NULL) {
        return;
    }

    for (i = 0; i < map->count; ++i) {
        free(map->entries[i].key);
        for (j = 0; j < map->entries[i].value_count; ++j) {
            free(map->entries[i].values[j]);
        }
        free(map->entries[i].values);
    }

    free(map->entries);
    map->entries = NULL;
    map->count = 0U;
    map->capacity = 0U;
}

static MapEntry *map_find_entry(Map *map, const char *key)
{
    size_t i;

    if (map == NULL || key == NULL) {
        return NULL;
    }

    for (i = 0; i < map->count; ++i) {
        if (strcmp(map->entries[i].key, key) == 0) {
            return &map->entries[i];
        }
    }

    return NULL;
}

static int grow_entries(Map *map)
{
    size_t new_capacity;
    MapEntry *new_entries;

    if (map == NULL) {
        return -1;
    }

    if (map->count < map->capacity) {
        return 0;
    }

    if (map->capacity == 0U) {
        new_capacity = 4U;
    } else {
        if (map->capacity > SIZE_MAX / 2U) {
            return -1;
        }
        new_capacity = map->capacity * 2U;
    }

    if (new_capacity > SIZE_MAX / sizeof(*new_entries)) {
        return -1;
    }

    new_entries = realloc(map->entries, new_capacity * sizeof(*new_entries));
    if (new_entries == NULL) {
        return -1;
    }

    map->entries = new_entries;
    map->capacity = new_capacity;
    return 0;
}

static int grow_values(MapEntry *entry)
{
    size_t new_capacity;
    char **new_values;

    if (entry == NULL) {
        return -1;
    }

    if (entry->value_count < entry->value_capacity) {
        return 0;
    }

    if (entry->value_capacity == 0U) {
        new_capacity = 4U;
    } else {
        if (entry->value_capacity > SIZE_MAX / 2U) {
            return -1;
        }
        new_capacity = entry->value_capacity * 2U;
    }

    if (new_capacity > SIZE_MAX / sizeof(*new_values)) {
        return -1;
    }

    new_values = realloc(entry->values, new_capacity * sizeof(*new_values));
    if (new_values == NULL) {
        return -1;
    }

    entry->values = new_values;
    entry->value_capacity = new_capacity;
    return 0;
}

static int map_add(Map *map, const char *key, const char *value)
{
    MapEntry *entry;
    char *value_copy;

    if (map == NULL || key == NULL || value == NULL) {
        return -1;
    }

    entry = map_find_entry(map, key);
    if (entry == NULL) {
        char *key_copy;

        if (grow_entries(map) != 0) {
            return -1;
        }

        key_copy = duplicate_string(key);
        if (key_copy == NULL) {
            return -1;
        }

        entry = &map->entries[map->count];
        entry->key = key_copy;
        entry->values = NULL;
        entry->value_count = 0U;
        entry->value_capacity = 0U;
        ++map->count;
    }

    if (grow_values(entry) != 0) {
        return -1;
    }

    value_copy = duplicate_string(value);
    if (value_copy == NULL) {
        return -1;
    }

    entry->values[entry->value_count] = value_copy;
    ++entry->value_count;
    return 0;
}

static int group_pairs(const KeyValuePair *pairs, size_t pair_count, Map *result)
{
    size_t i;

    if (result == NULL || (pairs == NULL && pair_count != 0U)) {
        return -1;
    }

    result->entries = NULL;
    result->count = 0U;
    result->capacity = 0U;

    for (i = 0; i < pair_count; ++i) {
        if (map_add(result, pairs[i].key, pairs[i].value) != 0) {
            map_destroy(result);
            return -1;
        }
    }

    return 0;
}

static int map_print(const Map *map)
{
    size_t i;
    size_t j;

    if (map == NULL) {
        return -1;
    }

    for (i = 0; i < map->count; ++i) {
        if (printf("%s: [", map->entries[i].key) < 0) {
            return -1;
        }

        for (j = 0; j < map->entries[i].value_count; ++j) {
            if (printf("%s%s",
                       j == 0U ? "" : ", ",
                       map->entries[i].values[j]) < 0) {
                return -1;
            }
        }

        if (printf("]\n") < 0) {
            return -1;
        }
    }

    return 0;
}

int main(void)
{
    const KeyValuePair pairs[] = {
        {"fruit", "apple"},
        {"color", "red"},
        {"fruit", "banana"},
        {"color", "blue"},
        {"fruit", "orange"},
        {"shape", "circle"}
    };
    Map map;
    int status;

    status = group_pairs(pairs, sizeof(pairs) / sizeof(pairs[0]), &map);
    if (status != 0) {
        fputs("Failed to group key-value pairs.\n", stderr);
        return EXIT_FAILURE;
    }

    status = map_print(&map);
    map_destroy(&map);

    if (status != 0) {
        fputs("Failed to print grouped pairs.\n", stderr);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}