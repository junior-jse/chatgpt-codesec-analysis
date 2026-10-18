/* Repair history (previous round):
 * - Flawfinder memcpy was replaced by strncpy, which Flawfinder flags with the same CWE (CWE-120).
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRING_LEN 64
#define MAX_VALUES 100
#define MAX_GROUPS 100

typedef struct {
    char *first;
    char *second;
} Tuple;

typedef struct {
    char *key;
    char **values;
    int value_count;
} Group;

int find_group_index(Group groups[], int group_count, const char *key) {
    if (groups == NULL || key == NULL) {
        return -1;
    }
    for (int i = 0; i < group_count; i++) {
        if (groups[i].key != NULL && strcmp(groups[i].key, key) == 0) {
            return i;
        }
    }
    return -1;
}

char *safe_strdup(const char *src) {
    if (src == NULL) {
        return NULL;
    }
    size_t len = strnlen(src, MAX_STRING_LEN);
    if (len >= MAX_STRING_LEN) {
        return NULL;
    }
    char *dest = malloc(len + 1);
    if (dest == NULL) {
        return NULL;
    }
    /* Possible weaknesses found:
     * Flawfinder strncpy: Easily used incorrectly; doesn't always \0-terminate or check for invalid pointers [MS-banned] (CWE-120). (risk 1, buffer)
     */
    strncpy(dest, src, len);
    dest[len] = '\0';
    return dest;
}

void free_groups(Group groups[], int group_count) {
    if (groups == NULL) {
        return;
    }
    for (int i = 0; i < group_count; i++) {
        if (groups[i].key != NULL) {
            free(groups[i].key);
            groups[i].key = NULL;
        }
        if (groups[i].values != NULL) {
            for (int j = 0; j < groups[i].value_count; j++) {
                if (groups[i].values[j] != NULL) {
                    free(groups[i].values[j]);
                    groups[i].values[j] = NULL;
                }
            }
            free(groups[i].values);
            groups[i].values = NULL;
        }
        groups[i].value_count = 0;
    }
}

int group_tuples(Tuple tuples[], int tuple_count, Group groups[], int max_groups) {
    int group_count = 0;
    
    if (tuples == NULL || groups == NULL || tuple_count < 0 || max_groups <= 0) {
        return -1;
    }
    
    for (int i = 0; i < tuple_count; i++) {
        if (tuples[i].first == NULL || tuples[i].second == NULL) {
            continue;
        }
        
        size_t first_len = strnlen(tuples[i].first, MAX_STRING_LEN);
        size_t second_len = strnlen(tuples[i].second, MAX_STRING_LEN);
        
        if (first_len >= MAX_STRING_LEN || second_len >= MAX_STRING_LEN) {
            continue;
        }
        
        int idx = find_group_index(groups, group_count, tuples[i].second);
        
        if (idx == -1) {
            if (group_count >= max_groups) {
                free_groups(groups, group_count);
                return -1;
            }
            
            groups[group_count].key = safe_strdup(tuples[i].second);
            if (groups[group_count].key == NULL) {
                free_groups(groups, group_count);
                return -1;
            }
            
            groups[group_count].values = malloc(MAX_VALUES * sizeof(char *));
            if (groups[group_count].values == NULL) {
                free(groups[group_count].key);
                groups[group_count].key = NULL;
                free_groups(groups, group_count);
                return -1;
            }
            
            memset(groups[group_count].values, 0, MAX_VALUES * sizeof(char *));
            groups[group_count].value_count = 0;
            idx = group_count;
            group_count++;
        }
        
        if (groups[idx].value_count < MAX_VALUES) {
            groups[idx].values[groups[idx].value_count] = safe_strdup(tuples[i].first);
            if (groups[idx].values[groups[idx].value_count] == NULL) {
                free_groups(groups, group_count);
                return -1;
            }
            groups[idx].value_count++;
        }
    }
    
    return group_count;
}

int main(void) {
    Tuple tuples[] = {
        {"apple", "fruit"},
        {"carrot", "vegetable"},
        {"banana", "fruit"},
        {"broccoli", "vegetable"},
        {"cherry", "fruit"},
        {"spinach", "vegetable"}
    };
    
    int tuple_count = sizeof(tuples) / sizeof(tuples[0]);
    Group groups[MAX_GROUPS];
    int group_count;
    
    memset(groups, 0, sizeof(groups));
    
    group_count = group_tuples(tuples, tuple_count, groups, MAX_GROUPS);
    
    if (group_count < 0) {
        fprintf(stderr, "Error: Failed to group tuples\n");
        return EXIT_FAILURE;
    }
    
    for (int i = 0; i < group_count; i++) {
        printf("%s: [", groups[i].key);
        for (int j = 0; j < groups[i].value_count; j++) {
            printf("%s", groups[i].values[j]);
            if (j < groups[i].value_count - 1) {
                printf(", ");
            }
        }
        printf("]\n");
    }
    
    free_groups(groups, group_count);
    
    return EXIT_SUCCESS;
}