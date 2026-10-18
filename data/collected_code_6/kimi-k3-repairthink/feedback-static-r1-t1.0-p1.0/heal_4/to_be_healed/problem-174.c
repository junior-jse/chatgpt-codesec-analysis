#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_STRING_LENGTH 1024

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
} Dict;

static void dict_init(Dict *dict) {
    dict->groups = NULL;
    dict->count = 0;
    dict->capacity = 0;
}

static char *duplicate_string(const char *str) {
    size_t len;
    char *copy;

    if (str == NULL) {
        return NULL;
    }

    len = strnlen(str, MAX_STRING_LENGTH);
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
    memcpy(copy, str, len);
    copy[len] = '\0';

    return copy;
}

static Group *find_or_create_group(Dict *dict, const char *key) {
    size_t i;
    Group *group;

    for (i = 0; i < dict->count; i++) {
        if (strcmp(dict->groups[i].key, key) == 0) {
            return &dict->groups[i];
        }
    }

    if (dict->count >= dict->capacity) {
        size_t new_capacity;
        Group *new_groups;

        if (dict->capacity > SIZE_MAX / 2) {
            return NULL;
        }
        new_capacity = dict->capacity == 0 ? 4 : dict->capacity * 2;
        if (new_capacity > SIZE_MAX / sizeof(Group)) {
            return NULL;
        }
        new_groups = realloc(dict->groups, new_capacity * sizeof(Group));
        if (new_groups == NULL) {
            return NULL;
        }
        dict->groups = new_groups;
        dict->capacity = new_capacity;
    }

    group = &dict->groups[dict->count];
    group->key = duplicate_string(key);
    if (group->key == NULL) {
        return NULL;
    }
    group->values = NULL;
    group->count = 0;
    group->capacity = 0;
    dict->count++;
    return group;
}

static int add_value_to_group(Group *group, const char *value) {
    if (group->count >= group->capacity) {
        size_t new_capacity;
        char **new_values;

        if (group->capacity > SIZE_MAX / 2) {
            return 0;
        }
        new_capacity = group->capacity == 0 ? 4 : group->capacity * 2;
        if (new_capacity > SIZE_MAX / sizeof(char *)) {
            return 0;
        }
        new_values = realloc(group->values, new_capacity * sizeof(char *));
        if (new_values == NULL) {
            return 0;
        }
        group->values = new_values;
        group->capacity = new_capacity;
    }

    group->values[group->count] = duplicate_string(value);
    if (group->values[group->count] == NULL) {
        return 0;
    }
    group->count++;
    return 1;
}

static int dict_add(Dict *dict, const char *key, const char *value) {
    Group *group = find_or_create_group(dict, key);
    if (group == NULL) {
        return 0;
    }
    return add_value_to_group(group, value);
}

static void dict_free(Dict *dict) {
    size_t i, j;
    for (i = 0; i < dict->count; i++) {
        free(dict->groups[i].key);
        for (j = 0; j < dict->groups[i].count; j++) {
            free(dict->groups[i].values[j]);
        }
        free(dict->groups[i].values);
    }
    free(dict->groups);
    dict->groups = NULL;
    dict->count = 0;
    dict->capacity = 0;
}

static void dict_print(const Dict *dict) {
    size_t i, j;
    for (i = 0; i < dict->count; i++) {
        printf("%s: [", dict->groups[i].key);
        for (j = 0; j < dict->groups[i].count; j++) {
            printf("%s", dict->groups[i].values[j]);
            if (j + 1 < dict->groups[i].count) {
                printf(", ");
            }
        }
        printf("]\n");
    }
}

int main(void) {
    Pair pairs[] = {
        {"fruit", "apple"},
        {"fruit", "banana"},
        {"vegetable", "carrot"},
        {"fruit", "cherry"},
        {"vegetable", "broccoli"},
        {"color", "red"},
        {"color", "blue"}
    };
    size_t num_pairs = sizeof(pairs) / sizeof(pairs[0]);
    size_t i;
    Dict dict;

    dict_init(&dict);

    for (i = 0; i < num_pairs; i++) {
        if (!dict_add(&dict, pairs[i].key, pairs[i].value)) {
            fprintf(stderr, "Error: failed to add pair\n");
            dict_free(&dict);
            return EXIT_FAILURE;
        }
    }

    dict_print(&dict);
    dict_free(&dict);

    return EXIT_SUCCESS;
}