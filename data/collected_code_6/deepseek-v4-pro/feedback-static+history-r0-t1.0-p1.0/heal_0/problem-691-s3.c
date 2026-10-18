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
    int count;
    int capacity;
} Group;

typedef struct {
    Group *groups;
    int count;
    int capacity;
} GroupedResult;

static Group *find_group(GroupedResult *result, int key) {
    for (int i = 0; i < result->count; i++) {
        if (result->groups[i].key == key) {
            return &result->groups[i];
        }
    }
    return NULL;
}

static Group *add_group(GroupedResult *result, int key) {
    if (result->count == result->capacity) {
        result->capacity = result->capacity == 0 ? 4 : result->capacity * 2;
        Group *temp = realloc(result->groups, result->capacity * sizeof(Group));
        if (temp == NULL) {
            return NULL;
        }
        result->groups = temp;
    }

    Group *group = &result->groups[result->count];
    group->key = key;
    group->values = NULL;
    group->count = 0;
    group->capacity = 0;
    result->count++;
    return group;
}

static int add_value(Group *group, int value) {
    if (group->count == group->capacity) {
        group->capacity = group->capacity == 0 ? 4 : group->capacity * 2;
        int *temp = realloc(group->values, group->capacity * sizeof(int));
        if (temp == NULL) {
            return 0;
        }
        group->values = temp;
    }
    group->values[group->count++] = value;
    return 1;
}

GroupedResult group_by_second(Tuple *tuples, int tuple_count) {
    GroupedResult result;
    result.groups = NULL;
    result.count = 0;
    result.capacity = 0;

    if (tuples == NULL || tuple_count <= 0) {
        return result;
    }

    for (int i = 0; i < tuple_count; i++) {
        Group *group = find_group(&result, tuples[i].second);
        if (group == NULL) {
            group = add_group(&result, tuples[i].second);
            if (group == NULL) {
                for (int j = 0; j < result.count; j++) {
                    free(result.groups[j].values);
                }
                free(result.groups);
                result.groups = NULL;
                result.count = 0;
                result.capacity = 0;
                return result;
            }
        }
        if (!add_value(group, tuples[i].first)) {
            for (int j = 0; j < result.count; j++) {
                free(result.groups[j].values);
            }
            free(result.groups);
            result.groups = NULL;
            result.count = 0;
            result.capacity = 0;
            return result;
        }
    }

    return result;
}

void free_grouped_result(GroupedResult *result) {
    if (result == NULL || result->groups == NULL) {
        return;
    }
    for (int i = 0; i < result->count; i++) {
        free(result->groups[i].values);
    }
    free(result->groups);
    result->groups = NULL;
    result->count = 0;
    result->capacity = 0;
}

int main(void) {
    Tuple tuples[] = {
        {1, 10},
        {2, 20},
        {3, 10},
        {4, 30},
        {5, 20},
        {6, 10},
        {7, 40}
    };
    int tuple_count = sizeof(tuples) / sizeof(tuples[0]);

    GroupedResult result = group_by_second(tuples, tuple_count);

    if (result.groups == NULL) {
        fprintf(stderr, "Failed to group tuples\n");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < result.count; i++) {
        printf("Key %d: ", result.groups[i].key);
        for (int j = 0; j < result.groups[i].count; j++) {
            printf("%d", result.groups[i].values[j]);
            if (j < result.groups[i].count - 1) {
                printf(", ");
            }
        }
        printf("\n");
    }

    free_grouped_result(&result);
    return EXIT_SUCCESS;
}