#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef enum {
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_STRING,
    TYPE_CHAR,
    TYPE_DOUBLE
} DataType;

typedef struct {
    DataType type;
    union {
        int i_val;
        float f_val;
        char *s_val;
        char c_val;
        double d_val;
    } data;
} TupleElement;

typedef struct {
    TupleElement *elements;
    size_t count;
    size_t capacity;
} Tuple;

void init_tuple(Tuple *t, size_t capacity) {
    t->elements = malloc(capacity * sizeof(TupleElement));
    t->count = 0;
    t->capacity = capacity;
}

void free_tuple(Tuple *t) {
    for (size_t i = 0; i < t->count; i++) {
        if (t->elements[i].type == TYPE_STRING) {
            free(t->elements[i].data.s_val);
        }
    }
    free(t->elements);
    t->elements = NULL;
    t->count = 0;
    t->capacity = 0;
}

bool add_int(Tuple *t, int val) {
    if (t->count >= t->capacity) return false;
    t->elements[t->count].type = TYPE_INT;
    t->elements[t->count].data.i_val = val;
    t->count++;
    return true;
}

bool add_float(Tuple *t, float val) {
    if (t->count >= t->capacity) return false;
    t->elements[t->count].type = TYPE_FLOAT;
    t->elements[t->count].data.f_val = val;
    t->count++;
    return true;
}

bool add_string(Tuple *t, const char *val) {
    if (t->count >= t->capacity) return false;
    t->elements[t->count].type = TYPE_STRING;
    t->elements[t->count].data.s_val = malloc(strlen(val) + 1);
    if (t->elements[t->count].data.s_val == NULL) return false;
    strcpy(t->elements[t->count].data.s_val, val);
    t->count++;
    return true;
}

bool add_char(Tuple *t, char val) {
    if (t->count >= t->capacity) return false;
    t->elements[t->count].type = TYPE_CHAR;
    t->elements[t->count].data.c_val = val;
    t->count++;
    return true;
}

bool add_double(Tuple *t, double val) {
    if (t->count >= t->capacity) return false;
    t->elements[t->count].type = TYPE_DOUBLE;
    t->elements[t->count].data.d_val = val;
    t->count++;
    return true;
}

void remove_by_type(Tuple *t, DataType type) {
    size_t write_idx = 0;
    for (size_t read_idx = 0; read_idx < t->count; read_idx++) {
        if (t->elements[read_idx].type != type) {
            if (write_idx != read_idx) {
                t->elements[write_idx] = t->elements[read_idx];
            }
            write_idx++;
        } else {
            if (t->elements[read_idx].type == TYPE_STRING) {
                free(t->elements[read_idx].data.s_val);
            }
        }
    }
    t->count = write_idx;
}

void print_tuple(const Tuple *t) {
    printf("(");
    for (size_t i = 0; i < t->count; i++) {
        switch (t->elements[i].type) {
            case TYPE_INT:
                printf("%d", t->elements[i].data.i_val);
                break;
            case TYPE_FLOAT:
                printf("%.2f", t->elements[i].data.f_val);
                break;
            case TYPE_STRING:
                printf("\"%s\"", t->elements[i].data.s_val);
                break;
            case TYPE_CHAR:
                printf("'%c'", t->elements[i].data.c_val);
                break;
            case TYPE_DOUBLE:
                printf("%.2lf", t->elements[i].data.d_val);
                break;
        }
        if (i < t->count - 1) {
            printf(", ");
        }
    }
    printf(")\n");
}

int main(void) {
    Tuple t;
    init_tuple(&t, 20);

    add_int(&t, 42);
    add_string(&t, "hello");
    add_float(&t, 3.14f);
    add_int(&t, 100);
    add_char(&t, 'A');
    add_string(&t, "world");
    add_double(&t, 2.718);
    add_int(&t, 7);
    add_float(&t, 1.5f);

    printf("Original tuple: ");
    print_tuple(&t);

    remove_by_type(&t, TYPE_INT);
    printf("After removing ints: ");
    print_tuple(&t);

    remove_by_type(&t, TYPE_STRING);
    printf("After removing strings: ");
    print_tuple(&t);

    free_tuple(&t);
    return 0;
}