#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char **items;
    size_t count;
} Tuple;

Tuple string_list_to_tuple(char **list, size_t count) {
    Tuple t;
    t.count = 0;
    t.items = NULL;

    if (list == NULL || count == 0) {
        return t;
    }

    t.items = (char **)malloc(count * sizeof(char *));
    if (t.items == NULL) {
        t.count = 0;
        return t;
    }

    for (size_t i = 0; i < count; ++i) {
        if (list[i] == NULL) {
            t.items[i] = NULL;
        } else {
            t.items[i] = (char *)malloc(strlen(list[i]) + 1);
            if (t.items[i] == NULL) {
                for (size_t j = 0; j < i; ++j) {
                    free(t.items[j]);
                }
                free(t.items);
                t.items = NULL;
                t.count = 0;
                return t;
            }
            strcpy(t.items[i], list[i]);
        }
    }

    t.count = count;
    return t;
}

void free_tuple(Tuple *t) {
    if (t == NULL) {
        return;
    }
    if (t->items != NULL) {
        for (size_t i = 0; i < t.count; ++i) {
            if (t->items[i] != NULL) {
                free(t->items[i]);
                t->items[i] = NULL;
            }
        }
        free(t->items);
        t->items = NULL;
    }
    t->count = 0;
}

int main(void) {
    char *list[] = {
        "apple",
        "banana",
        "cherry"
    };
    size_t count = sizeof(list) / sizeof(list[0]);

    Tuple t = string_list_to_tuple(list, count);

    if (t.items != NULL) {
        for (size_t i = 0; i < t.count; ++i) {
            if (t.items[i] != NULL) {
                printf("%s\n", t.items[i]);
            }
        }
    }

    free_tuple(&t);

    return 0;
}