#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define INITIAL_CAPACITY 4
#define MAX_STRING_LENGTH 4096

typedef struct {
    char *key;
    char **values;
    size_t value_count;
    size_t value_capacity;
} Group;

typedef struct {
    char *key;
    char *value;
} KeyValuePair;

typedef struct {
    Group *groups;
    size_t group_count;
    size_t group_capacity;
} GroupMap;

static size_t bounded_string_length(const char *str, size_t max_len) {
    size_t len = 0;

    if (str == NULL) {
        return 0;
    }

    while (len < max_len && str[len] != '\0') {
        len++;
    }

    return len;
}

static int safe_copy_string(char *dest, size_t dest_size, const char *src, size_t src_len) {
    if (dest == NULL || src == NULL || dest_size == 0) {
        return -1;
    }

    if (src_len >= dest_size) {
        return -1;
    }

    if (src_len > 0) {
        memcpy(dest, src, src_len);
    }
    dest[src_len] = '\0';

    return 0;
}

static char *duplicate_string(const char *str) {
    size_t len;
    size_t alloc_size;
    char *copy;

    if (str == NULL) {
        return NULL;
    }

    len = bounded_string_length(str, MAX_STRING_LENGTH);
    if (len >= MAX_STRING_LENGTH) {
        return NULL;
    }

    if (len == SIZE_MAX) {
        return NULL;
    }
    alloc_size = len + 1;

    copy = malloc(alloc_size);
    if (copy == NULL) {
        return NULL;
    }

    if (safe_copy_string(copy, alloc_size, str, len) != 0) {
        free(copy);
        return NULL;
    }

    return copy;
}

static void init_group_map(GroupMap *map) {
    if (map == NULL) {
        return;
    }
    map->groups = NULL;
    map->group_count = 0;
    map->group_capacity = 0;
}

static void free_group_map(GroupMap *map) {
    if (map == NULL) {
        return;
    }

    for (size_t i = 0; i < map->group_count; i++) {
        free(map->groups[i].key);
        for (size_t j = 0; j < map->groups[i].value_count; j++) {
            free(map->groups[i].values[j]);
        }
        free(map->groups[i].values);
    }
    free(map->groups);

    map->groups = NULL;
    map->group_count = 0;
    map->group_capacity = 0;
}

static Group *find_group(GroupMap *map, const char *key) {
    if (map == NULL || key == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < map->group_count; i++) {
        if (map->groups[i].key != NULL && strcmp(map->groups[i].key, key) == 0) {
            return &map->groups[i];
        }
    }
    return NULL;
}

static int add_value_to_group(Group *group, const char *value) {
    char *value_copy;

    if (group == NULL || value == NULL) {
        return -1;
    }

    if (group->value_count >= group->value_capacity) {
        size_t new_capacity;
        char **new_values;

        if (group->value_capacity > SIZE_MAX / 2) {
            return -1;
        }
        new_capacity = (group->value_capacity == 0)
                       ? INITIAL_CAPACITY
                       : group->value_capacity * 2;
        if (new_capacity > SIZE_MAX / sizeof(char *)) {
            return -1;
        }
        new_values = realloc(group->values, new_capacity * sizeof(char *));
        if (new_values == NULL) {
            return -1;
        }
        group->values = new_values;
        group->value_capacity = new_capacity;
    }

    value_copy = duplicate_string(value);
    if (value_copy == NULL) {
        return -1;
    }
    group->values[group->value_count] = value_copy;
    group->value_count++;

    return 0;
}

static int add_group(GroupMap *map, const char *key) {
    Group *new_group;

    if (map == NULL || key == NULL) {
        return -1;
    }

    if (map->group_count >= map->group_capacity) {
        size_t new_capacity;
        Group *new_groups;

        if (map->group_capacity > SIZE_MAX / 2) {
            return -1;
        }
        new_capacity = (map->group_capacity == 0)
                       ? INITIAL_CAPACITY
                       : map->group_capacity * 2;
        if (new_capacity > SIZE_MAX / sizeof(Group)) {
            return -1;
        }
        new_groups = realloc(map->groups, new_capacity * sizeof(Group));
        if (new_groups == NULL) {
            return -1;
        }
        map->groups = new_groups;
        map->group_capacity = new_capacity;
    }

    new_group = &map->groups[map->group_count];
    new_group->key = duplicate_string(key);
    if (new_group->key == NULL) {
        return -1;
    }
    new_group->values = NULL;
    new_group->value_count = 0;
    new_group->value_capacity = 0;
    map->group_count++;

    return 0;
}

static int group_key_value_pairs(GroupMap *map, const KeyValuePair *pairs, size_t pair_count) {
    if (map == NULL || (pairs == NULL && pair_count > 0)) {
        return -1;
    }

    for (size_t i = 0; i < pair_count; i++) {
        Group *group;

        if (pairs[i].key == NULL || pairs[i].value == NULL) {
            return -1;
        }

        group = find_group(map, pairs[i].key);
        if (group == NULL) {
            if (add_group(map, pairs[i].key) != 0) {
                return -1;
            }
            group = &map->groups[map->group_count - 1];
        }

        if (add_value_to_group(group, pairs[i].value) != 0) {
            return -1;
        }
    }

    return 0;
}

static void print_group_map(const GroupMap *map) {
    if (map == NULL) {
        return;
    }

    for (size_t i = 0; i < map->group_count; i++) {
        printf("%s: [", map->groups[i].key);
        for (size_t j = 0; j < map->groups[i].value_count; j++) {
            printf("%s", map->groups[i].values[j]);
            if (j < map->groups[i].value_count - 1) {
                printf(", ");
            }
        }
        printf("]\n");
    }
}

int main(void) {
    KeyValuePair pairs[] = {
        {"fruit", "apple"},
        {"vegetable", "carrot"},
        {"fruit", "banana"},
        {"vegetable", "broccoli"},
        {"fruit", "cherry"},
        {"grain", "rice"},
        {"grain", "wheat"}
    };
    size_t pair_count = sizeof(pairs) / sizeof(pairs[0]);
    GroupMap map;
    int result;

    init_group_map(&map);

    result = group_key_value_pairs(&map, pairs, pair_count);
    if (result != 0) {
        fprintf(stderr, "Error: Failed to group key-value pairs\n");
        free_group_map(&map);
        return EXIT_FAILURE;
    }

    print_group_map(&map);

    free_group_map(&map);

    return EXIT_SUCCESS;
}