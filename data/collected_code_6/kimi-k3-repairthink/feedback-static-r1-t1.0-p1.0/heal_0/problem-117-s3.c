#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <float.h>

typedef enum {
    TYPE_INT,
    TYPE_DOUBLE,
    TYPE_STRING
} ElementType;

typedef struct {
    ElementType type;
    union {
        int i;
        double d;
        char *s;
    } data;
} Element;

static int is_valid_float(const char *str) {
    if (str == NULL || *str == '\0') {
        return 0;
    }
    
    const char *p = str;
    while (isspace((unsigned char)*p)) {
        p++;
    }
    
    if (*p == '\0') {
        return 0;
    }
    
    if (*p == '+' || *p == '-') {
        p++;
    }
    
    int has_digits = 0;
    int has_dot = 0;
    
    while (*p != '\0') {
        if (isdigit((unsigned char)*p)) {
            has_digits = 1;
        } else if (*p == '.' && !has_dot) {
            has_dot = 1;
        } else if ((*p == 'e' || *p == 'E') && has_digits) {
            p++;
            if (*p == '+' || *p == '-') {
                p++;
            }
            if (!isdigit((unsigned char)*p)) {
                return 0;
            }
            while (isdigit((unsigned char)*p)) {
                p++;
            }
            break;
        } else {
            break;
        }
        p++;
    }
    
    if (!has_digits) {
        return 0;
    }
    
    while (isspace((unsigned char)*p)) {
        p++;
    }
    
    return *p == '\0';
}

static int string_to_float(const char *str, float *result) {
    if (str == NULL || result == NULL) {
        return 0;
    }
    
    if (!is_valid_float(str)) {
        return 0;
    }
    
    errno = 0;
    char *endptr = NULL;
    double val = strtod(str, &endptr);
    
    if (endptr == str || errno == ERANGE) {
        return 0;
    }
    
    if (val > FLT_MAX || val < -FLT_MAX) {
        return 0;
    }
    
    *result = (float)val;
    return 1;
}

size_t convert_to_float_list(const Element *elements, size_t count, float *output, size_t output_size) {
    if (elements == NULL || output == NULL || output_size == 0) {
        return 0;
    }
    
    size_t converted = 0;
    
    for (size_t i = 0; i < count && converted < output_size; i++) {
        switch (elements[i].type) {
            case TYPE_INT:
                output[converted] = (float)elements[i].data.i;
                converted++;
                break;
            case TYPE_DOUBLE:
                if (elements[i].data.d >= -FLT_MAX && elements[i].data.d <= FLT_MAX) {
                    output[converted] = (float)elements[i].data.d;
                    converted++;
                }
                break;
            case TYPE_STRING:
                if (string_to_float(elements[i].data.s, &output[converted])) {
                    converted++;
                }
                break;
        }
    }
    
    return converted;
}

int main(void) {
    Element elements[] = {
        {TYPE_INT, {.i = 42}},
        {TYPE_DOUBLE, {.d = 3.14159}},
        {TYPE_STRING, {.s = "2.71828"}},
        {TYPE_STRING, {.s = "-123.456"}},
        {TYPE_STRING, {.s = "not_a_number"}},
        {TYPE_INT, {.i = -100}},
        {TYPE_STRING, {.s = "1e10"}},
        {TYPE_STRING, {.s = "  7.5  "}}
    };
    
    size_t count = sizeof(elements) / sizeof(elements[0]);
    float results[16];
    size_t converted = convert_to_float_list(elements, count, results, sizeof(results) / sizeof(results[0]));
    
    printf("Converted %zu elements:\n", converted);
    for (size_t i = 0; i < converted; i++) {
        printf("%.6f\n", results[i]);
    }
    
    return 0;
}