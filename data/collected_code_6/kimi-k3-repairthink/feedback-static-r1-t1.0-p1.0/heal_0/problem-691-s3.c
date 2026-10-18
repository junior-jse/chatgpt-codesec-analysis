#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int first;
    int second;
} Tuple;

typedef struct {
    int key;
    int *values;
    size_t count;
    size_t capacity;
} Group;

typedef struct {
    Group *groups;
    size_t count;
    size_t capacity;
} GroupList;

int initGroupList(GroupList *list, size_t initial_capacity) {
    if (list == NULL || initial_capacity == 0) {
        return -1;
    }
    list->groups = malloc(initial_capacity * sizeof(Group));
    if (list->groups == NULL) {
        return -1;
    }
    list->count = 0;
    list->capacity = initial_capacity;
    return 0;
}

int findOrCreateGroup(GroupList *list, int key) {
    size_t i;
    Group *new_groups;
    
    if (list == NULL) {
        return -1;
    }
    
    for (i = 0; i < list->count; i++) {
        if (list->groups[i].key == key) {
            return (int)i;
        }
    }
    
    if (list->count >= list->capacity) {
        size_t new_capacity = list->capacity * 2;
        new_groups = realloc(list->groups, new_capacity * sizeof(Group));
        if (new_groups == NULL) {
            return -1;
        }
        list->groups = new_groups;
        list->capacity = new_capacity;
    }
    
    list->groups[list->count].key = key;
    list->groups[list->count].values = malloc(sizeof(int));
    if (list->groups[list->count].values == NULL) {
        return -1;
    }
    list->groups[list->count].count = 0;
    list->groups[list->count].capacity = 1;
    list->count++;
    
    return (int)(list->count - 1);
}

int addToGroup(GroupList *list, int group_index, int value) {
    Group *group;
    int *new_values;
    
    if (list == NULL || group_index < 0 || (size_t)group_index >= list->count) {
        return -1;
    }
    
    group = &list->groups[group_index];
    
    if (group->count >= group->capacity) {
        size_t new_capacity = group->capacity * 2;
        new_values = realloc(group->values, new_capacity * sizeof(int));
        if (new_values == NULL) {
            return -1;
        }
        group->values = new_values;
        group->capacity = new_capacity;
    }
    
    group->values[group->count] = value;
    group->count++;
    return 0;
}

int groupBySecond(const Tuple *tuples, size_t tuple_count, GroupList *result) {
    size_t i;
    int group_index;
    
    if (tuples == NULL || result == NULL) {
        return -1;
    }
    
    if (initGroupList(result, 4) != 0) {
        return -1;
    }
    
    for (i = 0; i < tuple_count; i++) {
        group_index = findOrCreateGroup(result, tuples[i].second);
        if (group_index < 0) {
            return -1;
        }
        if (addToGroup(result, group_index, tuples[i].first) != 0) {
            return -1;
        }
    }
    
    return 0;
}

void freeGroupList(GroupList *list) {
    size_t i;
    
    if (list == NULL) {
        return;
    }
    
    if (list->groups != NULL) {
        for (i = 0; i < list->count; i++) {
            free(list->groups[i].values);
            list->groups[i].values = NULL;
        }
        free(list->groups);
        list->groups = NULL;
    }
    
    list->count = 0;
    list->capacity = 0;
}

void printGroups(const GroupList *list) {
    size_t i, j;
    
    if (list == NULL) {
        return;
    }
    
    for (i = 0; i < list->count; i++) {
        printf("Key %d: [", list->groups[i].key);
        for (j = 0; j < list->groups[i].count; j++) {
            printf("%d", list->groups[i].values[j]);
            if (j < list->groups[i].count - 1) {
                printf(", ");
            }
        }
        printf("]\n");
    }
}

int main(void) {
    Tuple tuples[] = {
        {1, 10},
        {2, 20},
        {3, 10},
        {4, 30},
        {5, 20},
        {6, 10}
    };
    size_t tuple_count = sizeof(tuples) / sizeof(tuples[0]);
    GroupList result;
    int status;
    
    status = groupBySecond(tuples, tuple_count, &result);
    if (status != 0) {
        fprintf(stderr, "Error: Failed to group tuples\n");
        freeGroupList(&result);
        return EXIT_FAILURE;
    }
    
    printf("Grouped by second element:\n");
    printGroups(&result);
    
    freeGroupList(&result);
    return EXIT_SUCCESS;
}