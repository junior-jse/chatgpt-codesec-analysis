#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRING_LEN 64
#define MAX_VALUES 100
#define MAX_GROUPS 100

typedef struct {
    char first[MAX_STRING_LEN];
    char second[MAX_STRING_LEN];
} Tuple;

typedef struct {
    char key[MAX_STRING_LEN];
    char values[MAX_VALUES][MAX_STRING_LEN];
    int value_count;
} Group;

int find_group_index(Group groups[], int group_count, const char *key) {
    if (groups == NULL || key == NULL) {
        return -1;
    }
    for (int i = 0; i < group_count; i++) {
        if (strncmp(groups[i].key, key, MAX_STRING_LEN) == 0) {
            return i;
        }
    }
    return -1;
}

int group_tuples(Tuple tuples[], int tuple_count, Group groups[], int max_groups) {
    int group_count = 0;
    
    if (tuples == NULL || groups == NULL || tuple_count < 0 || max_groups <= 0) {
        return -1;
    }
    
    for (int i = 0; i < tuple_count; i++) {
        if (strnlen(tuples[i].second, MAX_STRING_LEN) >= MAX_STRING_LEN ||
            strnlen(tuples[i].first, MAX_STRING_LEN) >= MAX_STRING_LEN) {
            continue;
        }
        
        int idx = find_group_index(groups, group_count, tuples[i].second);
        
        if (idx == -1) {
            if (group_count >= max_groups) {
                return -1;
            }
            snprintf(groups[group_count].key, MAX_STRING_LEN, "%s", tuples[i].second);
            groups[group_count].value_count = 0;
            idx = group_count;
            group_count++;
        }
        
        if (groups[idx].value_count < MAX_VALUES) {
            snprintf(groups[idx].values[groups[idx].value_count], MAX_STRING_LEN, "%s", tuples[i].first);
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
    
    return EXIT_SUCCESS;
}