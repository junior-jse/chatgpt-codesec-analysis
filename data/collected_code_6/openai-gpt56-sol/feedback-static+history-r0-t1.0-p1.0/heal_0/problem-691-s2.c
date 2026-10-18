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

static void free_groups(Group *groups, size_t count)
{
    if (groups == NULL) {
        return;
    }

    for (size_t i = 0; i < count; ++i) {
        free(groups[i].values);
    }

    free(groups);
}

static int append_value(Group *group, int value)
{
    if (group == NULL) {
        return -1;
    }

    if (group->count == group->capacity) {
        size_t new_capacity = group->capacity == 0 ? 4 : group->capacity * 2;

        if (new_capacity < group->capacity ||
            new_capacity > SIZE_MAX / sizeof(*group->values)) {
            return -1;
        }

        int *new_values = realloc(group->values,
                                  new_capacity * sizeof(*group->values));
        if (new_values == NULL) {
            return -1;
        }

        group->values = new_values;
        group->capacity = new_capacity;
    }

    group->values[group->count++] = value;
    return 0;
}

static int group_tuples(const Tuple *tuples, size_t tuple_count,
                        Group **result, size_t *group_count)
{
    if (result == NULL || group_count == NULL ||
        (tuples == NULL && tuple_count != 0)) {
        return -1;
    }

    *result = NULL;
    *group_count = 0;

    if (tuple_count == 0) {
        return 0;
    }

    if (tuple_count > SIZE_MAX / sizeof(Group)) {
        return -1;
    }

    Group *groups = calloc(tuple_count, sizeof(*groups));
    if (groups == NULL) {
        return -1;
    }

    size_t count = 0;

    for (size_t i = 0; i < tuple_count; ++i) {
        size_t group_index = count;

        for (size_t j = 0; j < count; ++j) {
            if (groups[j].key == tuples[i].second) {
                group_index = j;
                break;
            }
        }

        if (group_index == count) {
            groups[count].key = tuples[i].second;
            ++count;
        }

        if (append_value(&groups[group_index], tuples[i].first) != 0) {
            free_groups(groups, count);
            return -1;
        }
    }

    *result = groups;
    *group_count = count;
    return 0;
}

static int print_groups(const Group *groups, size_t group_count)
{
    if (groups == NULL && group_count != 0) {
        return -1;
    }

    for (size_t i = 0; i < group_count; ++i) {
        if (printf("%d: [", groups[i].key) < 0) {
            return -1;
        }

        for (size_t j = 0; j < groups[i].count; ++j) {
            if (printf("%s%d", j == 0 ? "" : ", ", groups[i].values[j]) < 0) {
                return -1;
            }
        }

        if (printf("]\n") < 0) {
            return -1;
        }
    }

    return 0;
}

int main(void)
{
    const Tuple tuples[] = {
        {1, 10},
        {2, 20},
        {3, 10},
        {4, 30},
        {5, 20},
        {6, 10}
    };
    const size_t tuple_count = sizeof(tuples) / sizeof(tuples[0]);
    Group *groups = NULL;
    size_t group_count = 0;

    if (group_tuples(tuples, tuple_count, &groups, &group_count) != 0) {
        fputs("Failed to group tuples.\n", stderr);
        return EXIT_FAILURE;
    }

    if (print_groups(groups, group_count) != 0) {
        fputs("Failed to print groups.\n", stderr);
        free_groups(groups, group_count);
        return EXIT_FAILURE;
    }

    free_groups(groups, group_count);
    return EXIT_SUCCESS;
}