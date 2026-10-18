#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char first[50];
    char second[50];
} Tuple;

typedef struct {
    char key[50];
    char values[100][50];
    int value_count;
} Group;

int find_group_index(Group groups[], int group_count, const char *key) {
    for (int i = 0; i < group_count; i++) {
        if (strcmp(groups[i].key, key) == 0) {
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
        int idx = find_group_index(groups, group_count, tuples[i].second);
        
        if (idx == -1) {
            if (group_count >= max_groups) {
                return -1;
            }
            strncpy(groups[group_count].key, tuples[i].second, 49);
            groups[group_count].key[49] = '\0';
            groups[group_count].value_count = 0;
            idx = group_count;
            group_count++;
        }
        
        if (groups[idx].value_count < 100) {
            strncpy(groups[idx].values[groups[idx].value_count], tuples[i].first, 49);
            groups[idx].values[groups[idx].value_count][49] = '\0';
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
    Group groups[100];
    int group_count;
    
    group_count = group_tuples(tuples, tuple_count, groups, 100);
    
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