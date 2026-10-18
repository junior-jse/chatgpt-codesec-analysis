#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ENTRIES 1024
#define MAX_KEY_LEN 256
#define MAX_VAL_LEN 256
#define MAX_GROUPS 256
#define MAX_GROUP_ITEMS 256

typedef struct {
    char key[MAX_KEY_LEN];
    char value[MAX_VAL_LEN];
} KeyValuePair;

typedef struct {
    char key[MAX_KEY_LEN];
    char *items[MAX_GROUP_ITEMS];
    size_t count;
} Group;

typedef struct {
    Group groups[MAX_GROUPS];
    size_t group_count;
} Dictionary;

size_t find_or_create_group(Dictionary *dict, const char *key) {
    for (size_t i = 0; i < dict->group_count; i++) {
        if (strcmp(dict->groups[i].key, key) == 0) {
            return i;
        }
    }
    if (dict->group_count >= MAX_GROUPS) {
        fprintf(stderr, "Group limit reached\n");
        exit(EXIT_FAILURE);
    }
    size_t new_index = dict->group_count++;
    strncpy(dict->groups[new_index].key, key, MAX_KEY_LEN - 1);
    dict->groups[new_index].key[MAX_KEY_LEN - 1] = '\0';
    dict->groups[new_index].count = 0;
    return new_index;
}

void add_item_to_group(Group *group, const char *value) {
    if (group->count >= MAX_GROUP_ITEMS) {
        fprintf(stderr, "Group item limit reached\n");
        exit(EXIT_FAILURE);
    }
    char *item = malloc(MAX_VAL_LEN);
    if (item == NULL) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }
    strncpy(item, value, MAX_VAL_LEN - 1);
    item[MAX_VAL_LEN - 1] = '\0';
    group->items[group->count++] = item;
}

Dictionary group_key_value_pairs(KeyPair *pairs, size_t pair_count) {
    Dictionary dict;
    dict.group_count = 0;
    for (size_t i = 0; i < pair_count; i++) {
        size_t group_idx = find_or_create_group(&dict, pairs[i].key);
        add_item_to_group(&dict.groups[group_idx], pairs[i].value);
    }
    return dict;
}

void free_dictionary(Dictionary *dict) {
    for (size_t i = 0; i < dict->group_count; i++) {
        for (size_t j = 0; j < dict.groups[i].count; j++) {
            free(dict->groups[i].items[j]);
        }
    }
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