#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 4

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

static char *duplicate_string(const char *str) {
    size_t len;
    char *copy;
    
    if (str == NULL) {
        return NULL;
    }
    
    len = strlen(str) + 1;
    copy = malloc(len);
    if (copy == NULL) {
        return NULL;
    }
    
    memcpy(copy, str, len);
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
    size_t i;
    size_t j;
    
    if (map == NULL) {
        return;
    }
    
    for (i = 0; i < map->group_count; i++) {
        free(map->groups[i].key);
        for (j = 0; j < map->groups[i].value_count; j++) {
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
    size_t i;
    
    if (map == NULL || key == NULL) {
        return NULL;
    }
    
    for (i = 0; i < map->group_count; i++) {
        if (strcmp(map->groups[i].key, key) == 0) {
            return &map->groups[i];
        }
    }
    return NULL;
}

static int add_value_to_group(Group *group, const char *value) {
    char **new_values;
    size_t new_capacity;
    
    if (group == NULL || value == NULL) {
        return -1;
    }
    
    if (group->value_count >= group->value_capacity) {
        new_capacity = (group->value_capacity == 0) ? INITIAL_CAPACITY : group->value_capacity * 2;
        new_values = realloc(group->values, new_capacity * sizeof(char *));
        if (new_values == NULL) {
            return -1;
        }
        group->values = new_values;
        group->value_capacity = new_capacity;
    }
    
    group->values[group->value_count] = duplicate_string(value);
    if (group->values[group->value_count] == NULL) {
        return -1;
    }
    group->value_count++;
    
    return 0;
}

static int add_group(GroupMap *map, const char *key) {
    Group *new_groups;
    size_t new_capacity;
    Group *new_group;
    
    if (map == NULL || key == NULL) {
        return -1;
    }
    
    if (map->group_count >= map->group_capacity) {
        new_capacity = (map->group_capacity == 0) ? INITIAL_CAPACITY : map->group_capacity * 2;
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
    size_t i;
    Group *group;
    
    if (map == NULL || (pairs == NULL && pair_count > 0)) {
        return -1;
    }
    
    for (i = 0; i < pair_count; i++) {
        if (pairs[i].key == NULL || pairs[i].value == NULL) {
            return -1;
        }
        
        group = find_group(map, pairs[i].key);
        if (group == NULL) {
            if (add_group(map, pairs[i].key) != 0) {
                return -1;
            }
            group = find_group(map, pairs[i].key);
            if (group == NULL) {
                return -1;
            }
        }
        
        if (add_value_to_group(group, pairs[i].value) != 0) {
            return -1;
        }
    }
    
    return 0;
}

static void print_group_map(const GroupMap *map) {
    size_t i;
    size_t j;
    
    if (map == NULL) {
        return;
    }
    
    for (i = 0; i < map->group_count; i++) {
        printf("%s: [", map->groups[i].key);
        for (j = 0; j < map->groups[i].value_count; j++) {
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