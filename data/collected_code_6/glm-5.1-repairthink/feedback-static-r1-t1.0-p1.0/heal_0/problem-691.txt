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

int group_by_second(Tuple *tuples, int tuple_count, Group **groups, int *group_count) {
    if (tuples == NULL || groups == NULL || group_count == NULL || tuple_count <= 0) {
        return -1;
    }

    *groups = NULL;
    *group_count = 0;
    int capacity = 10;
    *groups = (Group *)malloc(capacity * sizeof(Group));
    if (*groups == NULL) {
        return -1;
    }

    for (int i = 0; i < tuple_count; i++) {
        int found = 0;
        for (int j = 0; j < *group_count; j++) {
            if ((*groups)[j].key == tuples[i].second) {
                if ((*groups)[j].count >= (*groups)[j].capacity) {
                    int new_capacity = (*groups)[j].capacity == 0 ? 4 : (*groups)[j].capacity * 2;
                    int *temp = (int *)realloc((*groups)[j].values, new_capacity * sizeof(int));
                    if (temp == NULL) {
                        for (int k = 0; k < *group_count; k++) {
                            free((*groups)[k].values);
                        }
                        free(*groups);
                        *groups = NULL;
                        *group_count = 0;
                        return -1;
                    }
                    (*groups)[j].values = temp;
                    (*groups)[j].capacity = new_capacity;
                }
                (*groups)[j].values[(*groups)[j].count++] = tuples[i].first;
                found = 1;
                break;
            }
        }

        if (!found) {
            if (*group_count >= capacity) {
                int new_capacity = capacity * 2;
                Group *temp = (Group *)realloc(*groups, new_capacity * sizeof(Group));
                if (temp == NULL) {
                    for (int k = 0; k < *group_count; k++) {
                        free((*groups)[k].values);
                    }
                    free(*groups);
                    *groups = NULL;
                    *group_count = 0;
                    return -1;
                }
                *groups = temp;
                capacity = new_capacity;
            }
            (*groups)[*group_count].key = tuples[i].second;
            (*groups)[*group_count].capacity = 4;
            (*groups)[*group_count].values = (int *)malloc((*groups)[*group_count].capacity * sizeof(int));
            if ((*groups)[*group_count].values == NULL) {
                for (int k = 0; k < *group_count; k++) {
                    free((*groups)[k].values);
                }
                free(*groups);
                *groups = NULL;
                *group_count = 0;
                return -1;
            }
            (*groups)[*group_count].values[0] = tuples[i].first;
            (*groups)[*group_count].count = 1;
            (*group_count)++;
        }
    }

    return 0;
}

int main(void) {
    Tuple tuples[] = {{1, 10}, {2, 20}, {3, 10}, {4, 30}, {5, 20}, {6, 10}};
    int tuple_count = sizeof(tuples) / sizeof(tuples[0]);

    Group *groups = NULL;
    int group_count = 0;

    if (group_by_second(tuples, tuple_count, &groups, &group_count) == 0) {
        for (int i = 0; i < group_count; i++) {
            printf("Key %d: [", groups[i].key);
            for (int j = 0; j < groups[i].count; j++) {
                printf("%d", groups[i].values[j]);
                if (j < groups[i].count - 1) {
                    printf(", ");
                }
            }
            printf("]\n");
            free(groups[i].values);
            groups[i].values = NULL;
        }
        free(groups);
        groups = NULL;
    }

    return 0;
}