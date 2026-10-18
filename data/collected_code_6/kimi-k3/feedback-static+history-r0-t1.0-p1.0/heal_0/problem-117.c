#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <math.h>

#define MAX_ELEMENTS 100
#define MAX_STR_LEN 64

typedef struct {
    char data[MAX_ELEMENTS][MAX_STR_LEN];
    size_t count;
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

void init_string_list(StringList *list) {
    if (list != NULL) {
        list->count = 0;
        memset(list->data, 0, sizeof(list->data));
    }
}

int add_string(StringList *list, const char *str) {
    if (list == NULL || str == NULL || list->count >= MAX_ELEMENTS) {
        return 0;
    }
    
    size_t len = strlen(str);
    if (len >= MAX_STR_LEN) {
        return 0;
    }
    
    strncpy(list->data[list->count], str, MAX_STR_LEN - 1);
    list->data[list->count][MAX_STR_LEN - 1] = '\0';
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
    float floats[MAX_ELEMENTS];
    
    init_string_list(&list);
    
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
    
    size_t converted = convert_to_float(&list, floats, MAX_ELEMENTS);
    
    printf("Total elements: %zu\n", list.count);
    printf("Converted elements: %zu\n", converted);
    print_float_array(floats, converted);
    
    return 0;
}