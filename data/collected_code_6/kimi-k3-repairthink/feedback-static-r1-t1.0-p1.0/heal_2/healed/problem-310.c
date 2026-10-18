#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#define TUPLE_MAX_ITEM_LEN 4096

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
    t->capacity = 8;
    t->count = 0;
    t->items = malloc(t->capacity * sizeof(char *));
    if (t->items == NULL) {
        free(t);
        return NULL;
    }
    return t;
}

static size_t bounded_strlen(const char *s, size_t max) {
    size_t n = 0;
    if (s == NULL) {
        return 0;
    }
    while (n < max && s[n] != '\0') {
        n++;
    }
    return n;
}

static int tuple_append_n(Tuple *t, const char *item, size_t len) {
    if (t == NULL || item == NULL) {
        return -1;
    }
    if (len > TUPLE_MAX_ITEM_LEN) {
        return -1;
    }
    if (t->count >= t->capacity) {
        size_t new_cap;
        char **new_items;
        if (t->capacity > SIZE_MAX / (2 * sizeof(char *))) {
            return -1;
        }
        new_cap = t->capacity * 2;
        new_items = realloc(t->items, new_cap * sizeof(char *));
        if (new_items == NULL) {
            return -1;
        }
        t->items = new_items;
        t->capacity = new_cap;
    }
    t->items[t->count] = malloc(len + 1);
    if (t->items[t->count] == NULL) {
        return -1;
    }
    if (len > 0) {
        memcpy(t->items[t->count], item, len);
    }
    t->items[t->count][len] = '\0';
    t->count++;
    return 0;
}

int tuple_append(Tuple *t, const char *item) {
    size_t len;
    if (t == NULL || item == NULL) {
        return -1;
    }
    len = bounded_strlen(item, TUPLE_MAX_ITEM_LEN + 1);
    if (len > TUPLE_MAX_ITEM_LEN) {
        return -1;
    }
    return tuple_append_n(t, item, len);
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
    int in_quotes = 0;

    for (;;) {
        if (*p == '"') {
            in_quotes = !in_quotes;
        }
        if ((*p == ',' && !in_quotes) || *p == '\0') {
            const char *tok_start = start;
            const char *tok_end = p;

            while (tok_start < tok_end && isspace((unsigned char)*tok_start)) {
                tok_start++;
            }
            while (tok_end > tok_start && isspace((unsigned char)tok_end[-1])) {
                tok_end--;
            }

            if (tuple_append_n(t, tok_start, (size_t)(tok_end - tok_start)) != 0) {
                tuple_free(t);
                return NULL;
            }

            if (*p == '\0') {
                break;
            }
            start = p + 1;
        }
        p++;
    }

    return t;
}

int main(void) {
    const char *input = "apple, banana, \"cherry, red\", date";
    Tuple *t = string_to_tuple(input);

    if (t == NULL) {
        fprintf(stderr, "Failed to convert string to tuple\n");
        return EXIT_FAILURE;
    }

    printf("(");
    for (size_t i = 0; i < t->count; i++) {
        printf("'%s'", t->items[i]);
        if (i < t->count - 1) {
            printf(", ");
        }
    }
    printf(")\n");

    tuple_free(t);
    return EXIT_SUCCESS;
}