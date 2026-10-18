#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    const char *first;
    const char *second;
} Tuple;

typedef struct {
    char *key;
    char **values;
    size_t value_count;
    size_t value_capacity;
} Group;

static char *dup_string(const char *src)
{
    size_t len;
    char *dst;

    if (src == NULL) {
        return NULL;
    }
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    len = strlen(src) + 1;
    dst = malloc(len);
    if (dst == NULL) {
        return NULL;
    }
    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(dst, src, len);
    return dst;
}

static int find_group_index(const Group groups[], size_t group_count, const char *key)
{
    size_t i;

    for (i = 0; i < group_count; i++) {
        if (strcmp(groups[i].key, key) == 0) {
            return (int)i;
        }
    }
    return -1;
}

static void free_groups(Group groups[], size_t group_count)
{
    size_t i;
    size_t j;

    if (groups == NULL) {
        return;
    }
    for (i = 0; i < group_count; i++) {
        free(groups[i].key);
        for (j = 0; j < groups[i].value_count; j++) {
            free(groups[i].values[j]);
        }
        free(groups[i].values);
    }
}

int group_tuples(const Tuple tuples[], size_t tuple_count, Group groups[], size_t max_groups)
{
    size_t group_count;
    size_t i;

    if (tuples == NULL || groups == NULL || max_groups == 0) {
        return -1;
    }

    group_count = 0;

    for (i = 0; i < tuple_count; i++) {
        int idx;

        if (tuples[i].first == NULL || tuples[i].second == NULL) {
            continue;
        }

        idx = find_group_index(groups, group_count, tuples[i].second);

        if (idx == -1) {
            if (group_count >= max_groups) {
                free_groups(groups, group_count);
                return -1;
            }
            groups[group_count].key = dup_string(tuples[i].second);
            if (groups[group_count].key == NULL) {
                free_groups(groups, group_count);
                return -1;
            }
            groups[group_count].values = NULL;
            groups[group_count].value_count = 0;
            groups[group_count].value_capacity = 0;
            idx = (int)group_count;
            group_count++;
        }

        if (groups[idx].value_count == groups[idx].value_capacity) {
            size_t new_capacity;
            char **new_values;

            new_capacity = (groups[idx].value_capacity == 0) ? 8 : groups[idx].value_capacity * 2;
            new_values = realloc(groups[idx].values, new_capacity * sizeof(*new_values));
            if (new_values == NULL) {
                free_groups(groups, group_count);
                return -1;
            }
            groups[idx].values = new_values;
            groups[idx].value_capacity = new_capacity;
        }

        groups[idx].values[groups[idx].value_count] = dup_string(tuples[i].first);
        if (groups[idx].values[groups[idx].value_count] == NULL) {
            free_groups(groups, group_count);
            return -1;
        }
        groups[idx].value_count++;
    }

    return (int)group_count;
}

int main(void)
{
    Tuple tuples[] = {
        {"apple", "fruit"},
        {"carrot", "vegetable"},
        {"banana", "fruit"},
        {"broccoli", "vegetable"},
        {"cherry", "fruit"},
        {"spinach", "vegetable"}
    };

    size_t tuple_count;
    size_t max_groups;
    Group *groups;
    int group_count;
    int i;

    tuple_count = sizeof(tuples) / sizeof(tuples[0]);
    max_groups = 100;
    groups = calloc(max_groups, sizeof(*groups));

    if (groups == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    group_count = group_tuples(tuples, tuple_count, groups, max_groups);

    if (group_count < 0) {
        fprintf(stderr, "Error: Failed to group tuples\n");
        free(groups);
        return EXIT_FAILURE;
    }

    for (i = 0; i < group_count; i++) {
        size_t j;

        printf("%s: [", groups[i].key);
        for (j = 0; j < groups[i].value_count; j++) {
            printf("%s", groups[i].values[j]);
            if (j + 1 < groups[i].value_count) {
                printf(", ");
            }
        }
        printf("]\n");
    }

    free_groups(groups, (size_t)group_count);
    free(groups);

    return EXIT_SUCCESS;
}