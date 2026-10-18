#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    void **items;
    size_t count;
    size_t capacity;
} List;

List *list_create(size_t initial_capacity) {
    List *list = malloc(sizeof(List));
    if (!list) return NULL;
    list->items = malloc(initial_capacity * sizeof(void *));
    if (!list->items) {
        free(list);
        return NULL;
    }
    list->count = 0;
    list->capacity = initial_capacity;
    return list;
}

int list_append(List *list, void *item) {
    if (!list || !list->items) return 0;
    if (list->count >= list->capacity) {
        size_t new_capacity = list->capacity == 0 ? 4 : list->capacity * 2;
        void **new_items = realloc(list->items, new_capacity * sizeof(void *));
        if (!new_items) return 0;
        list->items = new_items;
        list->capacity = new_capacity;
    }
    list->items[list->count++] = item;
    return 1;
}

void list_free(List *list) {
    if (!list) return;
    if (list->items) {
        for (size_t i = 0; i < list->count; i++) {
            free(list->items[i]);
        }
        free(list->items);
    }
    free(list);
}

int is_convertible_to_float(const char *str) {
    if (!str || *str == '\0') return 0;
    char *endptr;
    strtod(str, &endptr);
    if (endptr == str) return 0;
    for (; *endptr != '\0'; endptr++) {
        if (!isspace((unsigned char)*endptr)) return 0;
    }
    return 1;
}

float *convert_list_to_floats(List *list, size_t *out_count) {
    if (!list || !list->items || list->count == 0) {
        *out_count = 0;
        return NULL;
    }
    float *floats = malloc(list->count * sizeof(float));
    if (!floats) {
        *out_count = 0;
        return NULL;
    }
    size_t valid_count = 0;
    for (size_t i = 0; i < list->count; i++) {
        if (is_convertible_to_float((const char *)list->items[i])) {
            char *endptr;
            double val = strtod((const char *)list->items[i], &endptr);
            floats[valid_count++] = (float)val;
        }
    }
    if (valid_count == 0) {
        free(floats);
        *out_count = 0;
        return NULL;
    }
    if (valid_count < list->count) {
        float *shrunk = realloc(floats, valid_count * sizeof(float));
        if (shrunk) floats = shrunk;
    }
    *out_count = valid_count;
    return floats;
}

int main(void) {
    List *list = list_create(8);
    if (!list) return 1;
    
    const char *data[] = {"3.14", "not_a_float", "-42.5", "1.0e3", "  ", "0", "123abc"};
    
    for (size_t i = 0; i < sizeof(data) / sizeof(data[0]); i++) {
        char *item = malloc(strlen(data[i]) + 1);
        if (!item) {
            list_free(list);
            return 1;
        }
        strcpy(item, data[i]);
        if (!list_append(list, item)) {
            free(item);
            list_free(list);
            return 1;
        }
    }
    
    size_t float_count = 0;
    float *floats = convert_list_to_floats(list, &float_count);
    
    for (size_t i = 0; i < float_count; i++) {
        printf("%f\n", floats[i]);
    }
    
    free(floats);
    list_free(list);
    
    return 0;
}