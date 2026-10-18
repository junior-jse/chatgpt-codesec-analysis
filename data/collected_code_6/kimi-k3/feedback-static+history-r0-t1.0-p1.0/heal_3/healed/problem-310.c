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
    t->capacity = 8;
    t->count = 0;
    t->items = calloc(t->capacity, sizeof(char *));
    if (t->items == NULL) {
        free(t);
        return NULL;
    }
    return t;
}

int tuple_append(Tuple *t, const char *item, size_t item_len) {
    if (t == NULL || item == NULL) {
        return -1;
    }
    if (t->count >= t->capacity) {
        size_t new_cap = t->capacity * 2;
        char **new_items = realloc(t->items, new_cap * sizeof(char *));
        if (new_items == NULL) {
            return -1;
        }
        t->items = new_items;
        t->capacity = new_cap;
    }
    t->items[t->count] = malloc(item_len + 1);
    if (t->items[t->count] == NULL) {
        return -1;
    }
    strncpy(t->items[t->count], item, item_len);
    t->items[t->count][item_len] = '\0';
    t->count++;
    return 0;
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

static size_t trim_span(const char *start, size_t len, size_t *trimmed_len) {
    size_t left = 0;
    size_t right = len;
    
    while (left < right && isspace((unsigned char)start[left])) {
        left++;
    }
    while (right > left && isspace((unsigned char)start[right - 1])) {
        right--;
    }
    
    *trimmed_len = right - left;
    return left;
}

Tuple *string_to_tuple(const char *str, size_t str_len) {
    if (str == NULL) {
        return NULL;
    }
    
    Tuple *t = tuple_create();
    if (t == NULL) {
        return NULL;
    }
    
    size_t start = 0;
    int in_quotes = 0;
    
    for (size_t i = 0; i <= str_len; i++) {
        if (i < str_len && str[i] == '"') {
            in_quotes = !in_quotes;
        } else if (i == str_len || (str[i] == ',' && !in_quotes)) {
            size_t len = i - start;
            size_t trimmed_len;
            size_t offset = trim_span(str + start, len, &trimmed_len);
            
            if (tuple_append(t, str + start + offset, trimmed_len) != 0) {
                tuple_free(t);
                return NULL;
            }
            start = i + 1;
        }
    }
    
    return t;
}

int main(void) {
    const char *input = "apple, banana, \"cherry, red\", date";
    Tuple *t = string_to_tuple(input, strlen(input));
    
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