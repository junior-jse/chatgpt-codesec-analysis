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

int init_group_list(GroupList *list, size_t initial_capacity) {
    if (list == NULL || initial_capacity == 0) {
        return -1;
    }
    list->groups = (Group *)malloc(initial_capacity * sizeof(Group));
    if (list->groups == NULL) {
        return -1;
    }
    list->count = 0;
    list->capacity = initial_capacity;
    return 0;
}

int add_to_group(GroupList *list, int key, int value) {
    if (list == NULL) {
        return -1;
    }
    
    size_t i;
    for (i = 0; i < list->count; i++) {
        if (list->groups[i].key == key) {
            break;
        }
    }
    
    if (i == list->count) {
        if (list->count >= list->capacity) {
            size_t new_capacity = list->capacity * 2;
            Group *new_groups = (Group *)realloc(list->groups, new_capacity * sizeof(Group));
            if (new_groups == NULL) {
                return -1;
            }
            list->groups = new_groups;
            list->capacity = new_capacity;
        }
        
        list->groups[list->count].key = key;
        list->groups[list->count].values = (int *)malloc(sizeof(int));
        if (list->groups[list->count].values == NULL) {
            return -1;
        }
        list->groups[list->count].values[0] = value;
        list->groups[list->count].count = 1;
        list->groups[list->count].capacity = 1;
        list->count++;
    } else {
        if (list->groups[i].count >= list->groups[i].capacity) {
            size_t new_capacity = list->groups[i].capacity * 2;
            int *new_values = (int *)realloc(list->groups[i].values, new_capacity * sizeof(int));
            if (new_values == NULL) {
                return -1;
            }
            list->groups[i].values = new_values;
            list->groups[i].capacity = new_capacity;
        }
        list->groups[i].values[list->groups[i].count] = value;
        list->groups[i].count++;
    }
    
    return 0;
}

int group_tuples(const Tuple *tuples, size_t tuple_count, GroupList *result) {
    if (tuples == NULL || result == NULL || tuple_count == 0) {
        return -1;
    }
    
    if (init_group_list(result, 4) != 0) {
        return -1;
    }
    
    size_t i;
    for (i = 0; i < tuple_count; i++) {
        if (add_to_group(result, tuples[i].second, tuples[i].first) != 0) {
            size_t j;
            for (j = 0; j < result->count; j++) {
                free(result->groups[j].values);
            }
            free(result->groups);
            result->groups = NULL;
            result->count = 0;
            result->capacity = 0;
            return -1;
        }
    }
    
    return 0;
}

void free_group_list(GroupList *list) {
    if (list == NULL || list->groups == NULL) {
        return;
    }
    
    size_t i;
    for (i = 0; i < list->count; i++) {
        free(list->groups[i].values);
        list->groups[i].values = NULL;
    }
    free(list->groups);
    list->groups = NULL;
    list->count = 0;
    list->capacity = 0;
}

void print_groups(const GroupList *list) {
    if (list == NULL) {
        return;
    }
    
    size_t i, j;
    for (i = 0; i < list->count; i++) {
        printf("%d: [", list->groups[i].key);
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
        {1, 2},
        {3, 2},
        {5, 3},
        {7, 2},
        {9, 3},
        {11, 4}
    };
    size_t tuple_count = sizeof(tuples) / sizeof(tuples[0]);
    
    GroupList result;
    if (group_tuples(tuples, tuple_count, &result) != 0) {
        fprintf(stderr, "Error: Failed to group tuples\n");
        return EXIT_FAILURE;
    }
    
    printf("Grouped by second element:\n");
    print_groups(&result);
    
    free_group_list(&result);
    
    return EXIT_SUCCESS;
}