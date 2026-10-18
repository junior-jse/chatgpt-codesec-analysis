#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

#define INITIAL_CAPACITY 10
#define MAX_INPUT_SIZE 256

typedef enum {
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_STRING
} ElementType;

typedef struct {
    ElementType type;
    union {
        int int_val;
        float float_val;
        char *str_val;
    } data;
} Element;

typedef struct {
    Element *elements;
    size_t count;
    size_t capacity;
} ElementList;

int init_list(ElementList *list, size_t initial_capacity) {
    if (list == NULL || initial_capacity == 0) {
        return -1;
    }
    
    list->elements = malloc(initial_capacity * sizeof(Element));
    if (list->elements == NULL) {
        return -1;
    }
    
    list->count = 0;
    list->capacity = initial_capacity;
    return 0;
}

void free_list(ElementList *list) {
    if (list == NULL || list->elements == NULL) {
        return;
    }
    
    for (size_t i = 0; i < list->count; i++) {
        if (list->elements[i].type == TYPE_STRING && 
            list->elements[i].data.str_val != NULL) {
            free(list->elements[i].data.str_val);
            list->elements[i].data.str_val = NULL;
        }
    }
    
    free(list->elements);
    list->elements = NULL;
    list->count = 0;
    list->capacity = 0;
}

int add_int(ElementList *list, int value) {
    if (list == NULL || list->count >= list->capacity) {
        return -1;
    }
    
    list->elements[list->count].type = TYPE_INT;
    list->elements[list->count].data.int_val = value;
    list->count++;
    return 0;
}

int add_float(ElementList *list, float value) {
    if (list == NULL || list->count >= list->capacity) {
        return -1;
    }
    
    list->elements[list->count].type = TYPE_FLOAT;
    list->elements[list->count].data.float_val = value;
    list->count++;
    return 0;
}

int add_string(ElementList *list, const char *value) {
    if (list == NULL || value == NULL || list->count >= list->capacity) {
        return -1;
    }
    
    size_t len = strlen(value);
    if (len == 0 || len >= MAX_INPUT_SIZE) {
        return -1;
    }
    
    char *str_copy = malloc(len + 1);
    if (str_copy == NULL) {
        return -1;
    }
    
    strncpy(str_copy, value, len);
    str_copy[len] = '\0';
    
    list->elements[list->count].type = TYPE_STRING;
    list->elements[list->count].data.str_val = str_copy;
    list->count++;
    return 0;
}

int convert_to_float(const Element *elem, float *result) {
    if (elem == NULL || result == NULL) {
        return -1;
    }
    
    char *endptr = NULL;
    long long_val;
    float float_val;
    
    switch (elem->type) {
        case TYPE_INT:
            *result = (float)elem->data.int_val;
            return 0;
            
        case TYPE_FLOAT:
            *result = elem->data.float_val;
            return 0;
            
        case TYPE_STRING:
            if (elem->data.str_val == NULL) {
                return -1;
            }
            
            errno = 0;
            long_val = strtol(elem->data.str_val, &endptr, 10);
            
            if (errno == 0 && endptr != elem->data.str_val && *endptr == '\0') {
                if (long_val >= INT_MIN && long_val <= INT_MAX) {
                    *result = (float)long_val;
                    return 0;
                }
            }
            
            errno = 0;
            float_val = strtof(elem->data.str_val, &endptr);
            
            if (errno == 0 && endptr != elem->data.str_val && *endptr == '\0') {
                *result = float_val;
                return 0;
            }
            
            return -1;
            
        default:
            return -1;
    }
}

size_t convert_all_to_float(const ElementList *list, float *output, size_t output_size) {
    if (list == NULL || output == NULL || output_size == 0) {
        return 0;
    }
    
    size_t converted = 0;
    size_t limit = (list->count < output_size) ? list->count : output_size;
    
    for (size_t i = 0; i < limit; i++) {
        if (convert_to_float(&list->elements[i], &output[converted]) == 0) {
            converted++;
        }
    }
    
    return converted;
}

int main(void) {
    ElementList list;
    float results[INITIAL_CAPACITY];
    size_t converted_count;
    
    if (init_list(&list, INITIAL_CAPACITY) != 0) {
        fprintf(stderr, "Failed to initialize list\n");
        return EXIT_FAILURE;
    }
    
    if (add_int(&list, 42) != 0) {
        fprintf(stderr, "Failed to add integer\n");
        free_list(&list);
        return EXIT_FAILURE;
    }
    
    if (add_float(&list, 3.14f) != 0) {
        fprintf(stderr, "Failed to add float\n");
        free_list(&list);
        return EXIT_FAILURE;
    }
    
    if (add_string(&list, "123") != 0) {
        fprintf(stderr, "Failed to add string\n");
        free_list(&list);
        return EXIT_FAILURE;
    }
    
    if (add_string(&list, "45.67") != 0) {
        fprintf(stderr, "Failed to add string\n");
        free_list(&list);
        return EXIT_FAILURE;
    }
    
    if (add_string(&list, "invalid") != 0) {
        fprintf(stderr, "Failed to add string\n");
        free_list(&list);
        return EXIT_FAILURE;
    }
    
    if (add_string(&list, "-89") != 0) {
        fprintf(stderr, "Failed to add string\n");
        free_list(&list);
        return EXIT_FAILURE;
    }
    
    converted_count = convert_all_to_float(&list, results, INITIAL_CAPACITY);
    
    printf("Converted %zu elements to float:\n", converted_count);
    for (size_t i = 0; i < converted_count; i++) {
        printf("%.2f ", results[i]);
    }
    printf("\n");
    
    free_list(&list);
    return EXIT_SUCCESS;
}