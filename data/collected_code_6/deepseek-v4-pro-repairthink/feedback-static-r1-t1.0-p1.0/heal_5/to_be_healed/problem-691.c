#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_GROUPS 256
#define MAX_KEY_LEN 64
#define MAX_VAL_LEN 64

typedef struct {
    const char *first;
    const char *second;
} Tuple;

typedef struct {
    char *key;
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char *values[MAX_GROUPS];
    int count;
} Group;

static size_t safe_strnlen(const char *s, size_t maxlen) {
    size_t i;
    if (s == NULL) return 0;
    for (i = 0; i < maxlen && s[i] != '\0'; i++);
    return i;
}

static int find_group(const Group *groups, int group_count, const char *key) {
    int i;
    for (i = 0; i < group_count; i++) {
        if (groups[i].key != NULL && strcmp(groups[i].key, key) == 0) {
            return i;
        }
    }
    return -1;
}

static int is_valid_length(const char *str, size_t max_len) {
    return str != NULL && safe_strnlen(str, max_len) < max_len;
}

static void free_groups(Group *groups, int group_count) {
    int i, j;
    if (groups == NULL) return;
    for (i = 0; i < group_count; i++) {
        free(groups[i].key);
        groups[i].key = NULL;
        for (j = 0; j < groups[i].count; j++) {
            free(groups[i].values[j]);
            groups[i].values[j] = NULL;
        }
        groups[i].count = 0;
    }
}

int group_tuples(Tuple *tuples, int tuple_count, Group *groups, int max_groups) {
    int group_count = 0;
    int i;

    if (tuples == NULL || groups == NULL || tuple_count < 0 || max_groups <= 0) {
        return -1;
    }

    for (i = 0; i < tuple_count; i++) {
        if (!is_valid_length(tuples[i].first, MAX_VAL_LEN) ||
            !is_valid_length(tuples[i].second, MAX_KEY_LEN)) {
            free_groups(groups, group_count);
            return -3;
        }

        int idx = find_group(groups, group_count, tuples[i].second);

        if (idx >= 0) {
            if (groups[idx].count >= MAX_GROUPS) {
                continue;
            }
            size_t len_first = safe_strnlen(tuples[i].first, MAX_VAL_LEN);
            char *val_copy = malloc(len_first + 1);
            if (val_copy == NULL) {
                free_groups(groups, group_count);
                return -1;
            }
            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
            memcpy(val_copy, tuples[i].first, len_first + 1);
            groups[idx].values[groups[idx].count] = val_copy;
            groups[idx].count++;
        } else {
            if (group_count >= max_groups) {
                free_groups(groups, group_count);
                return -2;
            }
            size_t len_second = safe_strnlen(tuples[i].second, MAX_KEY_LEN);
            char *key_copy = malloc(len_second + 1);
            if (key_copy == NULL) {
                free_groups(groups, group_count);
                return -1;
            }
            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
            memcpy(key_copy, tuples[i].second, len_second + 1);

            size_t len_first = safe_strnlen(tuples[i].first, MAX_VAL_LEN);
            char *val_copy = malloc(len_first + 1);
            if (val_copy == NULL) {
                free(key_copy);
                free_groups(groups, group_count);
                return -1;
            }
            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
            memcpy(val_copy, tuples[i].first, len_first + 1);

            groups[group_count].key = key_copy;
            groups[group_count].values[0] = val_copy;
            groups[group_count].count = 1;
            group_count++;
        }
    }

    return group_count;
}

int main(void) {
    Tuple tuples[] = {
        {"apple", "fruit"},
        {"banana", "fruit"},
        {"carrot", "vegetable"},
        {"dog", "animal"},
        {"elephant", "animal"},
        {"fig", "fruit"},
        {"grape", "fruit"}
    };
    int tuple_count = sizeof(tuples) / sizeof(tuples[0]);
    Group groups[MAX_GROUPS];
    int result;
    int i, j;

    memset(groups, 0, sizeof(groups));

    result = group_tuples(tuples, tuple_count, groups, MAX_GROUPS);

    if (result < 0) {
        printf("Error during grouping\n");
        free_groups(groups, MAX_GROUPS);
        return 1;
    }

    for (i = 0; i < result; i++) {
        printf("%s: ", groups[i].key);
        for (j = 0; j < groups[i].count; j++) {
            if (j > 0) {
                printf(", ");
            }
            printf("%s", groups[i].values[j]);
        }
        printf("\n");
    }

    free_groups(groups, result);
    return 0;
}