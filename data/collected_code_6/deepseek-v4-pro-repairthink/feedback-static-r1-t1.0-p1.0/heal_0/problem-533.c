#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef enum {
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_DOUBLE,
    TYPE_CHAR,
    TYPE_STRING
} DataType;

typedef union {
    int i_val;
    float f_val;
    double d_val;
    char c_val;
    char *s_val;
} DataValue;

typedef struct {
    DataType type;
    DataValue value;
} TupleElement;

typedef struct {
    TupleElement *elements;
    size_t size;
} Tuple;

Tuple *create_tuple(void) {
    Tuple *t = (Tuple *)malloc(sizeof(Tuple));
    if (t == NULL) {
        return NULL;
    }
    t->elements = NULL;
    t->size = 0;
    return t;
}

int add_int(Tuple *t, int value) {
    TupleElement *new_elements = (TupleElement *)realloc(t->elements, (t->size + 1) * sizeof(TupleElement));
    if (new_elements == NULL) {
        return -1;
    }
    t->elements = new_elements;
    t->elements[t->size].type = TYPE_INT;
    t->elements[t->size].value.i_val = value;
    t->size++;
    return 0;
}

int add_float(Tuple *t, float value) {
    TupleElement *new_elements = (TupleElement *)realloc(t->elements, (t->size + 1) * sizeof(TupleElement));
    if (new_elements == NULL) {
        return -1;
    }
    t->elements = new_elements;
    t->elements[t->size].type = TYPE_FLOAT;
    t->elements[t->size].value.f_val = value;
    t->size++;
    return 0;
}

int add_double(Tuple *t, double value) {
    TupleElement *new_elements = (TupleElement *)realloc(t->elements, (t->size + 1) * sizeof(TupleElement));
    if (new_elements == NULL) {
        return -1;
    }
    t->elements = new_elements;
    t->elements[t->size].type = TYPE_DOUBLE;
    t->elements[t->size].value.d_val = value;
    t->size++;
    return 0;
}

int add_char(Tuple *t, char value) {
    TupleElement *new_elements = (TupleElement *)realloc(t->elements, (t->size + 1) * sizeof(TupleElement));
    if (new_elements == NULL) {
        return -1;
    }
    t->elements = new_elements;
    t->elements[t->size].type = TYPE_CHAR;
    t->elements[t->size].value.c_val = value;
    t->size++;
    return 0;
}

int add_string(Tuple *t, const char *value) {
    TupleElement *new_elements = (TupleElement *)realloc(t->elements, (t->size + 1) * sizeof(TupleElement));
    if (new_elements == NULL) {
        return -1;
    }
    t->elements = new_elements;
    t->elements[t->size].type = TYPE_STRING;
    t->elements[t->size].value.s_val = (char *)malloc(strlen(value) + 1);
    if (t->elements[t->size].value.s_val == NULL) {
        return -1;
    }
    strcpy(t->elements[t->size].value.s_val, value);
    t->size++;
    return 0;
}

Tuple *remove_type_elements(const Tuple *input, DataType type_to_remove) {
    if (input == NULL) {
        return NULL;
    }

    Tuple *result = create_tuple();
    if (result == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < input->size; i++) {
        if (input->elements[i].type != type_to_remove) {
            TupleElement *new_elements = (TupleElement *)realloc(result->elements, (result->size + 1) * sizeof(TupleElement));
            if (new_elements == NULL) {
                free_tuple(result);
                return NULL;
            }
            result->elements = new_elements;
            result->elements[result->size] = input->elements[i];
            
            if (input->elements[i].type == TYPE_STRING) {
                result->elements[result->size].value.s_val = (char *)malloc(strlen(input->elements[i].value.s_val) + 1);
                if (result->elements[result->size].value.s_val == NULL) {
                    free_tuple(result);
                    return NULL;
                }
                strcpy(result->elements[result->size].value.s_val, input->elements[i].value.s_val);
            }
            
            result->size++;
        }
    }

    return result;
}

void free_tuple(Tuple *t) {
    if (t == NULL) {
        return;
    }
    for (size_t i = 0; i < t->size; i++) {
        if (t->elements[i].type == TYPE_STRING && t->elements[i].value.s_val != NULL) {
            free(t->elements[i].value.s_val);
        }
    }
    free(t->elements);
    free(t);
}

void print_tuple(const Tuple *t) {
    if (t == NULL) {
        printf("NULL\n");
        return;
    }
    printf("(");
    for (size_t i = 0; i < t->size; i++) {
        switch (t->elements[i].type) {
            case TYPE_INT:
                printf("%d", t->elements[i].value.i_val);
                break;
            case TYPE_FLOAT:
                printf("%f", t->elements[i].value.f_val);
                break;
            case TYPE_DOUBLE:
                printf("%lf", t->elements[i].value.d_val);
                break;
            case TYPE_CHAR:
                printf("'%c'", t->elements[i].value.c_val);
                break;
            case TYPE_STRING:
                printf("\"%s\"", t->elements[i].value.s_val);
                break;
        }
        if (i < t->size - 1) {
            printf(", ");
        }
    }
    printf(")\n");
}

int main(void) {
    Tuple *t = create_tuple();
    if (t == NULL) {
        fprintf(stderr, "Failed to create tuple\n");
        return 1;
    }

    add_int(t, 42);
    add_string(t, "hello");
    add_float(t, 3.14f);
    add_char(t, 'A');
    add_int(t, 100);
    add_string(t, "world");
    add_double(t, 2.718);

    printf("Original tuple: ");
    print_tuple(t);

    Tuple *no_ints = remove_type_elements(t, TYPE_INT);
    printf("After removing ints: ");
    print_tuple(no_ints);

    Tuple *no_strings = remove_type_elements(t, TYPE_STRING);
    printf("After removing strings: ");
    print_tuple(no_strings);

    Tuple *no_floats = remove_type_elements(t, TYPE_FLOAT);
    printf("After removing floats: ");
    print_tuple(no_floats);

    Tuple *no_doubles = remove_type_elements(t, TYPE_DOUBLE);
    printf("After removing doubles: ");
    print_tuple(no_doubles);

    Tuple *no_chars = remove_type_elements(t, TYPE_CHAR);
    printf("After removing chars: ");
    print_tuple(no_chars);

    free_tuple(t);
    free_tuple(no_ints);
    free_tuple(no_strings);
    free_tuple(no_floats);
    free_tuple(no_doubles);
    free_tuple(no_chars);

    return 0;
}