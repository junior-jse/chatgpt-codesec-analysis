#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <math.h>

#define INITIAL_CAPACITY 16
#define MAX_STR_LEN 64

typedef struct {
    char **data;
    size_t count;
    size_t capacity;
} StringList;

int is_convertible_to_float(const char *str) {
    if (str == NULL || *str == '\0') {
        return 0;
    }

    char *endptr;
    errno = 0;
    strtof(str, &endptr);

    if (errno == ERANGE) {
        return 0;
    }

    if (endptr == str) {
        return 0;
    }

    while (*endptr != '\0') {
        if (!isspace((unsigned char)*endptr)) {
            return 0;
        }
        endptr++;
    }

    return 1;
}

size_t convert_to_float(const StringList *list, float *output, size_t max_output) {
    if (list == NULL || output == NULL || max_output == 0) {
        return 0;
    }

    size_t converted = 0;

    for (size_t i = 0; i < list->count && converted < max_output; i++) {
        if (is_convertible_to_float(list->data[i])) {
            char *endptr;
            errno = 0;
            float value = strtof(list->data[i], &endptr);

            if (errno != ERANGE && endptr != list->data[i]) {
                output[converted] = value;
                converted++;
            }
        }
    }

    return converted;
}

int init_string_list(StringList *list) {
    if (list == NULL) {
        return 0;
    }

    list->data = calloc(INITIAL_CAPACITY, sizeof(char *));
    if (list->data == NULL) {
        list->count = 0;
        list->capacity = 0;
        return 0;
    }

    list->count = 0;
    list->capacity = INITIAL_CAPACITY;

    return 1;
}

void free_string_list(StringList *list) {
    if (list == NULL) {
        return;
    }

    for (size_t i = 0; i < list->count; i++) {
        free(list->data[i]);
    }

    free(list->data);
    list->data = NULL;
    list->count = 0;
    list->capacity = 0;
}

int add_string(StringList *list, const char *str) {
    if (list == NULL || str == NULL || list->data == NULL) {
        return 0;
    }

    size_t len = strnlen(str, MAX_STR_LEN);
    if (len >= MAX_STR_LEN) {
        return 0;
    }

    if (list->count >= list->capacity) {
        size_t new_capacity = list->capacity * 2;
        char **new_data = realloc(list->data, new_capacity * sizeof(char *));
        if (new_data == NULL) {
            return 0;
        }
        list->data = new_data;
        list->capacity = new_capacity;
    }

    size_t dest_size = len + 1;
    char *copy = malloc(dest_size);
    if (copy == NULL) {
        return 0;
    }

    snprintf(copy, dest_size, "%s", str);

    list->data[list->count] = copy;
    list->count++;

    return 1;
}

void print_float_array(const float *arr, size_t count) {
    if (arr == NULL) {
        return;
    }

    printf("Converted floats: ");
    for (size_t i = 0; i < count; i++) {
        printf("%.2f ", arr[i]);
    }
    printf("\n");
}

int main(void) {
    StringList list;
    float *floats = NULL;

    if (!init_string_list(&list)) {
        fprintf(stderr, "Failed to initialize string list\n");
        return EXIT_FAILURE;
    }

    add_string(&list, "3.14");
    add_string(&list, "42");
    add_string(&list, "-2.5");
    add_string(&list, "hello");
    add_string(&list, "1e3");
    add_string(&list, "0.001");
    add_string(&list, "abc");
    add_string(&list, "7.77");
    add_string(&list, "  5.5  ");
    add_string(&list, "");
    add_string(&list, "12.34.56");
    add_string(&list, "100");

    if (list.count > 0) {
        floats = malloc(list.count * sizeof(float));
        if (floats == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            free_string_list(&list);
            return EXIT_FAILURE;
        }
    }

    size_t converted = convert_to_float(&list, floats, list.count);

    printf("Total elements: %zu\n", list.count);
    printf("Converted elements: %zu\n", converted);
    print_float_array(floats, converted);

    free(floats);
    free_string_list(&list);

    return EXIT_SUCCESS;
}