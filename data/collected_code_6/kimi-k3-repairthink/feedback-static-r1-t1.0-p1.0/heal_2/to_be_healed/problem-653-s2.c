#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRING_LENGTH 4096

typedef struct {
    char *key;
    char *value;
} Pair;

typedef struct {
    char *key;
    char **values;
    size_t count;
    size_t capacity;
} Group;

typedef struct {
    Group *groups;
    size_t count;
    size_t capacity;
} Map;

static void free_map(Map *map)
{
    size_t i, j;
    if (map == NULL) {
        return;
    }
    for (i = 0; i < map->count; i++) {
        free(map->groups[i].key);
        for (j = 0; j < map->groups[i].count; j++) {
            free(map->groups[i].values[j]);
        }
        free(map->groups[i].values);
    }
    free(map->groups);
    map->groups = NULL;
    map->count = 0;
    map->capacity = 0;
}

static size_t bounded_strlen(const char *s)
{
    size_t len = 0;
    while (len < MAX_STRING_LENGTH && s[len] != '\0') {
        len++;
    }
    return len;
}

static char *duplicate_string(const char *s)
{
    size_t len;
    char *copy;

    if (s == NULL) {
        return NULL;
    }

    len = bounded_strlen(s);
    if (len == MAX_STRING_LENGTH) {
        return NULL;
    }

    copy = malloc(len + 1);
    if (copy == NULL) {
        return NULL;
    }

    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(copy, s, len);
    copy[len] = '\0';
    return copy;
}

static int add_value_to_group(Group *group, const char *value)
{
    char *value_copy;

    if (group == NULL || value == NULL) {
        return -1;
    }

    if (group->count >= group->capacity) {
        size_t new_capacity = (group->capacity == 0) ? 4 : group->capacity * 2;
        char **new_values = realloc(group->values, new_capacity * sizeof(char *));
        if (new_values == NULL) {
            return -1;
        }
        group->values = new_values;
        group->capacity = new_capacity;
    }

    value_copy = duplicate_string(value);
    if (value_copy == NULL) {
        return -1;
    }

    group->values[group->count] = value_copy;
    group->count++;
    return 0;
}

static int add_pair(Map *map, const char *key, const char *value)
{
    size_t i;
    char *key_copy;

    if (map == NULL || key == NULL || value == NULL) {
        return -1;
    }

    for (i = 0; i < map->count; i++) {
        if (strcmp(map->groups[i].key, key) == 0) {
            return add_value_to_group(&map->groups[i], value);
        }
    }

    if (map->count >= map->capacity) {
        size_t new_capacity = (map->capacity == 0) ? 4 : map->capacity * 2;
        Group *new_groups = realloc(map->groups, new_capacity * sizeof(Group));
        if (new_groups == NULL) {
            return -1;
        }
        map->groups = new_groups;
        map->capacity = new_capacity;
    }

    key_copy = duplicate_string(key);
    if (key_copy == NULL) {
        return -1;
    }

    map->groups[map->count].key = key_copy;
    map->groups[map->count].values = NULL;
    map->groups[map->count].count = 0;
    map->groups[map->count].capacity = 0;

    if (add_value_to_group(&map->groups[map->count], value) != 0) {
        free(key_copy);
        map->groups[map->count].key = NULL;
        return -1;
    }

    map->count++;
    return 0;
}

static int group_pairs(const Pair *pairs, size_t pair_count, Map *map)
{
    size_t i;
    int result = 0;

    if (map == NULL) {
        return -1;
    }

    map->groups = NULL;
    map->count = 0;
    map->capacity = 0;

    if (pairs == NULL && pair_count > 0) {
        return -1;
    }

    for (i = 0; i < pair_count; i++) {
        if (add_pair(map, pairs[i].key, pairs[i].value) != 0) {
            result = -1;
            break;
        }
    }

    if (result != 0) {
        free_map(map);
    }

    return result;
}

int main(void)
{
    Pair pairs[] = {
        {"fruit", "apple"},
        {"vegetable", "carrot"},
        {"fruit", "banana"},
        {"vegetable", "broccoli"},
        {"fruit", "cherry"},
        {"dairy", "milk"}
    };
    size_t pair_count = sizeof(pairs) / sizeof(pairs[0]);
    Map map;
    size_t i, j;

    if (group_pairs(pairs, pair_count, &map) != 0) {
        fprintf(stderr, "Error: failed to group pairs\n");
        return EXIT_FAILURE;
    }

    for (i = 0; i < map.count; i++) {
        printf("%s: ", map.groups[i].key);
        for (j = 0; j < map.groups[i].count; j++) {
            printf("%s", map.groups[i].values[j]);
            if (j + 1 < map.groups[i].count) {
                printf(", ");
            }
        }
        printf("\n");
    }

    free_map(&map);
    return EXIT_SUCCESS;
}