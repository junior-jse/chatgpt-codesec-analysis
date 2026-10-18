#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_CHAR
} DataType;

typedef struct {
    DataType type;
    union {
        int int_val;
        float float_val;
        char char_val;
    } value;
} TupleElement;

typedef struct {
    TupleElement *elements;
    size_t size;
    size_t capacity;
} Tuple;

Tuple* tuple_create(size_t capacity) {
    Tuple *t = (Tuple*)malloc(sizeof(Tuple));
    if (!t) return NULL;
    t->elements = (TupleElement*)malloc(capacity * sizeof(TupleElement));
    if (!t->elements) {
        free(t);
        return NULL;
    }
    t->size = 0;
    t->capacity = capacity;
    return t;
}

void tuple_destroy(Tuple *t) {
    if (t) {
        free(t->elements);
        free(t);
    }
}

int tuple_add(Tuple *t, DataType type, void *val) {
    if (!t || t->size >= t->capacity) return 0;
    t->elements[t->size].type = type;
    switch (type) {
        case TYPE_INT:
            t->elements[t->size].value.int_val = *(int*)val;
            break;
        case TYPE_FLOAT:
            t->elements[t->size].value.float_val = *(float*)val;
            break;
        case TYPE_CHAR:
            t->elements[t->size].value.char_val = *(char*)val;
            break;
    }
    t->size++;
    return 1;
}

Tuple* tuple_remove_by_type(const Tuple *t, DataType type_to_remove) {
    if (!t) return NULL;

    Tuple *new_t = tuple_create(t->capacity);
    if (!new_t) return NULL;

    for (size_t i = 0; i < t->size; i++) {
        if (t->elements[i].type != type_to_remove) {
            if (!tuple_add(new_t, t->elements[i].type, &t->elements[i].value)) {
                tuple_destroy(new_t);
                return NULL;
            }
        }
    }
    return new_t;
}

void tuple_print(const Tuple *t) {
    if (!t) return;
    printf("(");
    for (size_t i = 0; i < t->size; i++) {
        switch (t->elements[i].type) {
            case TYPE_INT:
                printf("%d", t->elements[i].value.int_val);
                break;
            case TYPE_FLOAT:
                printf("%f", t->elements[i].value.float_val);
                break;
            case TYPE_CHAR:
                printf("'%c'", t->elements[i].value.char_val);
                break;
        }
        if (i < t->size - 1) printf(", ");
    }
    printf(")\n");
}

int main(void) {
    Tuple *t = tuple_create(10);
    if (!t) return 1;

    int v1 = 10, v2 = 20;
    float v3 = 3.14f, v4 = 6.28f;
    char v5 = 'a', v6 = 'b', v7 = 'c';

    tuple_add(t, TYPE_INT, &v1);
    tuple_add(t, TYPE_FLOAT, &v3);
    tuple_add(t, TYPE_CHAR, &v5);
    tuple_add(t, TYPE_INT, &v2);
    tuple_add(t, TYPE_FLOAT, &v4);
    tuple_add(t, TYPE_CHAR, &v6);
    tuple_add(t, TYPE_CHAR, &v7);

    printf("Original: ");
    tuple_print(t);

    Tuple *t_no_int = tuple_remove_by_type(t, TYPE_INT);
    if (!t_no_int) {
        tuple_destroy(t);
        return 1;
    }
    printf("Without TYPE_INT: ");
    tuple_print(t_no_int);

    Tuple *t_no_char = tuple_remove_by_type(t, TYPE_CHAR);
    if (!t_no_char) {
        tuple_destroy(t);
        tuple_destroy(t_no_int);
        return 1;
    }
    printf("Without TYPE_CHAR: ");
    tuple_print(t_no_char);

    tuple_destroy(t);
    tuple_destroy(t_no_int);
    tuple_destroy(t_no_char);

    return 0;
}