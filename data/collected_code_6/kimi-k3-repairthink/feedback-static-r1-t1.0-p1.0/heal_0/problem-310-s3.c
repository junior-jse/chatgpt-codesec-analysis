#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    char **items;
    size_t count;
    size_t capacity;
} Tuple;

Tuple *tuple_create(void) {
    Tuple *t = malloc(sizeof(Tuple));
    if (t == NULL) {
        return NULL;
    }
    t->items = NULL;
    t->count = 0;
    t->capacity = 0;
    return t;
}

int tuple_append(Tuple *t, const char *str, size_t len) {
    if (t == NULL || str == NULL) {
        return 0;
    }
    if (t->count >= t->capacity) {
        size_t new_cap = (t->capacity == 0) ? 8 : t->capacity * 2;
        char **new_items = realloc(t->items, new_cap * sizeof(char *));
        if (new_items == NULL) {
            return 0;
        }
        t->items = new_items;
        t->capacity = new_cap;
    }
    char *item = malloc(len + 1);
    if (item == NULL) {
        return 0;
    }
    memcpy(item, str, len);
    item[len] = '\0';
    t->items[t->count] = item;
    t->count++;
    return 1;
}

void tuple_free(Tuple *t) {
    if (t == NULL) {
        return;
    }
    for (size_t i = 0; i < t->count; i++) {
        free(t->items[i]);
    }
    free(t->items);
    free(t);
}

Tuple *string_to_tuple(const char *str) {
    if (str == NULL) {
        return NULL;
    }
    Tuple *t = tuple_create();
    if (t == NULL) {
        return NULL;
    }
    const char *start = str;
    const char *p = str;
    while (*p != '\0') {
        if (*p == ',') {
            if (!tuple_append(t, start, (size_t)(p - start))) {
                tuple_free(t);
                return NULL;
            }
            start = p + 1;
        }
        p++;
    }
    if (!tuple_append(t, start, (size_t)(p - start))) {
        tuple_free(t);
        return NULL;
    }
    return t;
}

void tuple_print(const Tuple *t) {
    if (t == NULL) {
        printf("()\n");
        return;
    }
    printf("(");
    for (size_t i = 0; i < t->count; i++) {
        printf("\"%s\"", t->items[i]);
        if (i < t->count - 1) {
            printf(", ");
        }
    }
    printf(")\n");
}

int main(void) {
    const char *input = "apple,banana,cherry,date";
    Tuple *t = string_to_tuple(input);
    if (t == NULL) {
        fprintf(stderr, "Failed to convert string to tuple\n");
        return EXIT_FAILURE;
    }
    tuple_print(t);
    tuple_free(t);
    return EXIT_SUCCESS;
}