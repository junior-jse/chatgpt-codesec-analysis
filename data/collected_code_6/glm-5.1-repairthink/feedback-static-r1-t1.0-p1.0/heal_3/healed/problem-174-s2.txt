#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *key;
    char *value;
} KeyValuePair;

typedef struct {
    char *key;
    char **items;
    size_t count;
    size_t capacity;
} Group;

typedef struct {
    Group *groups;
    size_t group_count;
    size_t group_capacity;
} Dictionary;

size_t find_or_create_group(Dictionary *dict, const char *key) {
    for (size_t i = 0; i < dict->group_count; i++) {
        if (strcmp(dict->groups[i].key, key) == 0) {
            return i;
        }
    }
    if (dict->group_count >= dict->group_capacity) {
        size_t new_cap = dict->group_capacity == 0 ? 4 : dict->group_capacity * 2;
        Group *new_groups = realloc(dict->groups, new_cap * sizeof(Group));
        if (!new_groups) {
            perror("realloc failed");
            exit(EXIT_FAILURE);
        }
        dict->groups = new_groups;
        dict->group_capacity = new_cap;
    }
    size_t new_index = dict->group_count++;
    size_t key_len = snprintf(NULL, 0, "%s", key);
    dict->groups[new_index].key = malloc(key_len + 1);
    if (!dict->groups[new_index].key) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }
    snprintf(dict->groups[new_index].key, key_len + 1, "%s", key);
    dict->groups[new_index].items = NULL;
    dict->groups[new_index].count = 0;
    dict->groups[new_index].capacity = 0;
    return new_index;
}

void add_item_to_group(Group *group, const char *value) {
    if (group->count >= group->capacity) {
        size_t new_cap = group->capacity == 0 ? 4 : group->capacity * 2;
        char **new_items = realloc(group->items, new_cap * sizeof(char *));
        if (!new_items) {
            perror("realloc failed");
            exit(EXIT_FAILURE);
        }
        group->items = new_items;
        group->capacity = new_cap;
    }
    size_t val_len = snprintf(NULL, 0, "%s", value);
    group->items[group->count] = malloc(val_len + 1);
    if (!group->items[group->count]) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }
    snprintf(group->items[group->count], val_len + 1, "%s", value);
    group->count++;
}

Dictionary group_key_value_pairs(KeyValuePair *pairs, size_t pair_count) {
    Dictionary dict;
    dict.groups = NULL;
    dict.group_count = 0;
    dict.group_capacity = 0;
    for (size_t i = 0; i < pair_count; i++) {
        size_t group_idx = find_or_create_group(&dict, pairs[i].key);
        add_item_to_group(&dict.groups[group_idx], pairs[i].value);
    }
    return dict;
}

void free_dictionary(Dictionary *dict) {
    for (size_t i = 0; i < dict->group_count; i++) {
        for (size_t j = 0; j < dict->groups[i].count; j++) {
            free(dict->groups[i].items[j]);
        }
        free(dict->groups[i].items);
        free(dict->groups[i].key);
    }
    free(dict->groups);
    dict->groups = NULL;
    dict->group_count = 0;
    dict->group_capacity = 0;
}

void print_dictionary(const Dictionary *dict) {
    for (size_t i = 0; i < dict->group_count; i++) {
        printf("%s: [", dict->groups[i].key);
        for (size_t j = 0; j < dict->groups[i].count; j++) {
            printf("\"%s\"", dict->groups[i].items[j]);
            if (j < dict->groups[i].count - 1) {
                printf(", ");
            }
        }
        printf("]\n");
    }
}

int main(void) {
    KeyValuePair pairs[] = {
        {"fruit", "apple"},
        {"color", "red"},
        {"fruit", "banana"},
        {"color", "green"},
        {"fruit", "cherry"},
        {"color", "blue"}
    };
    size_t pair_count = sizeof(pairs) / sizeof(pairs[0]);
    
    Dictionary dict = group_key_value_pairs(pairs, pair_count);
    print_dictionary(&dict);
    free_dictionary(&dict);
    
    return 0;
}