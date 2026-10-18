#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_DOUBLE,
    TYPE_CHAR,
    TYPE_STRING,
    TYPE_COUNT
} DataType;

typedef union {
    int int_val;
    float float_val;
    double double_val;
    char char_val;
    char *str_val;
} DataValue;

typedef struct {
    DataType type;
    DataValue value;
} TupleElement;

typedef struct {
    TupleElement *elements;
    size_t size;
    size_t capacity;
} Tuple;

static const char* type_to_name(DataType type) {
    switch (type) {
        case TYPE_INT:    return "int";
        case TYPE_FLOAT:  return "float";
        case TYPE_DOUBLE: return "double";
        case TYPE_CHAR:   return "char";
        case TYPE_STRING: return "string";
        default:          return "unknown";
    }
}

static DataType name_to_type(const char *name) {
    if (strcmp(name, "int") == 0) return TYPE_INT;
    if (strcmp(name, "float") == 0) return TYPE_FLOAT;
    if (strcmp(name, "double") == 0) return TYPE_DOUBLE;
    if (strcmp(name, "char") == 0) return TYPE_CHAR;
    if (strcmp(name, "string") == 0) return TYPE_STRING;
    return TYPE_COUNT;
}

static int tuple_init(Tuple *tuple) {
    if (!tuple) return 0;
    tuple->elements = NULL;
    tuple->size = 0;
    tuple->capacity = 0;
    return 1;
}

static void tuple_free(Tuple *tuple) {
    if (!tuple || !tuple->elements) return;
    for (size_t i = 0; i < tuple->size; i++) {
        if (tuple->elements[i].type == TYPE_STRING && tuple->elements[i].value.str_val) {
            free(tuple->elements[i].value.str_val);
        }
    }
    free(tuple->elements);
    tuple_init(tuple);
}

static int tuple_push_int(Tuple *tuple, int val) {
    if (!tuple) return 0;
    if (tuple->size == tuple->capacity) {
        size_t new_cap = tuple->capacity ? tuple->capacity * 2 : 4;
        TupleElement *new_elements = realloc(tuple->elements, new_cap * sizeof(TupleElement));
        if (!new_elements) return 0;
        tuple->elements = new_elements;
        tuple->capacity = new_cap;
    }
    tuple->elements[tuple->size].type = TYPE_INT;
    tuple->elements[tuple->size].value.int_val = val;
    tuple->size++;
    return 1;
}

static int tuple_push_float(Tuple *tuple, float val) {
    if (!tuple) return 0;
    if (tuple->size == tuple->capacity) {
        size_t new_cap = tuple->capacity ? tuple->capacity * 2 : 4;
        TupleElement *new_elements = realloc(tuple->elements, new_cap * sizeof(TupleElement));
        if (!new_elements) return 0;
        tuple->elements = new_elements;
        tuple->capacity = new_cap;
    }
    tuple->elements[tuple->size].type = TYPE_FLOAT;
    tuple->elements[tuple->size].value.float_val = val;
    tuple->size++;
    return 1;
}

static int tuple_push_double(Tuple *tuple, double val) {
    if (!tuple) return 0;
    if (tuple->size == tuple->capacity) {
        size_t new_cap = tuple->capacity ? tuple->capacity * 2 : 4;
        TupleElement *new_elements = realloc(tuple->elements, new_cap * sizeof(TupleElement));
        if (!new_elements) return 0;
        tuple->elements = new_elements;
        tuple->capacity = new_cap;
    }
    tuple->elements[tuple->size].type = TYPE_DOUBLE;
    tuple->elements[tuple->size].value.double_val = val;
    tuple->size++;
    return 1;
}

static int tuple_push_char(Tuple *tuple, char val) {
    if (!tuple) return 0;
    if (tuple->size == tuple->capacity) {
        size_t new_cap = tuple->capacity ? tuple->capacity * 2 : 4;
        TupleElement *new_elements = realloc(tuple->elements, new_cap * sizeof(TupleElement));
        if (!new_elements) return 0;
        tuple->elements = new_elements;
        tuple->capacity = new_cap;
    }
    tuple->elements[tuple->size].type = TYPE_CHAR;
    tuple->elements[tuple->size].value.char_val = val;
    tuple->size++;
    return 1;
}

static int tuple_push_string(Tuple *tuple, const char *str) {
    if (!tuple || !str) return 0;
    if (tuple->size == tuple->capacity) {
        size_t new_cap = tuple->capacity ? tuple->capacity * 2 : 4;
        TupleElement *new_elements = realloc(tuple->elements, new_cap * sizeof(TupleElement));
        if (!new_elements) return 0;
        tuple->elements = new_elements;
        tuple->capacity = new_cap;
    }
    char *copy = malloc(strlen(str) + 1);
    if (!copy) return 0;
    strcpy(copy, str);
    tuple->elements[tuple->size].type = TYPE_STRING;
    tuple->elements[tuple->size].value.str_val = copy;
    tuple->size++;
    return 1;
}

static void tuple_print(const Tuple *tuple) {
    if (!tuple) {
        printf("()\n");
        return;
    }
    printf("(");
    for (size_t i = 0; i < tuple->size; i++) {
        if (i > 0) printf(", ");
        switch (tuple->elements[i].type) {
            case TYPE_INT:
                printf("%d", tuple->elements[i].value.int_val);
                break;
            case TYPE_FLOAT:
                printf("%f", tuple->elements[i].value.float_val);
                break;
            case TYPE_DOUBLE:
                printf("%lf", tuple->elements[i].value.double_val);
                break;
            case TYPE_CHAR:
                printf("'%c'", tuple->elements[i].value.char_val);
                break;
            case TYPE_STRING:
                printf("\"%s\"", tuple->elements[i].value.str_val);
                break;
            default:
                printf("?");
                break;
        }
    }
    printf(")\n");
}

static Tuple tuple_remove_type(const Tuple *source, DataType type) {
    Tuple result;
    tuple_init(&result);
    if (!source || !source->elements || source->size == 0) {
        return result;
    }

    size_t count = 0;
    for (size_t i = 0; i < source->size; i++) {
        if (source->elements[i].type != type) count++;
    }

    if (count == 0) {
        return result;
    }

    result.elements = malloc(count * sizeof(TupleElement));
    if (!result.elements) {
        tuple_init(&result);
        return result;
    }
    result.capacity = count;
    result.size = 0;

    for (size_t i = 0; i < source->size; i++) {
        if (source->elements[i].type == type) continue;
        result.elements[result.size].type = source->elements[i].type;
        switch (source->elements[i].type) {
            case TYPE_INT:
                result.elements[result.size].value.int_val = source->elements[i].value.int_val;
                break;
            case TYPE_FLOAT:
                result.elements[result.size].value.float_val = source->elements[i].value.float_val;
                break;
            case TYPE_DOUBLE:
                result.elements[result.size].value.double_val = source->elements[i].value.double_val;
                break;
            case TYPE_CHAR:
                result.elements[result.size].value.char_val = source->elements[i].value.char_val;
                break;
            case TYPE_STRING: {
                char *copy = malloc(strlen(source->elements[i].value.str_val) + 1);
                if (!copy) {
                    for (size_t j = 0; j < result.size; j++) {
                        if (result.elements[j].type == TYPE_STRING) {
                            free(result.elements[j].value.str_val);
                        }
                    }
                    free(result.elements);
                    tuple_init(&result);
                    return result;
                }
                strcpy(copy, source->elements[i].value.str_val);
                result.elements[result.size].value.str_val = copy;
                break;
            }
            default:
                break;
        }
        result.size++;
    }
    return result;
}

int main(void) {
    Tuple tuple;
    tuple_init(&tuple);

    tuple_push_int(&tuple, 42);
    tuple_push_float(&tuple, 3.14f);
    tuple_push_double(&tuple, 2.718281828);
    tuple_push_char(&tuple, 'A');
    tuple_push_string(&tuple, "hello");
    tuple_push_int(&tuple, 100);
    tuple_push_float(&tuple, 1.5f);
    tuple_push_string(&tuple, "world");

    printf("Original tuple: ");
    tuple_print(&tuple);

    Tuple no_ints = tuple_remove_type(&tuple, TYPE_INT);
    printf("After removing ints: ");
    tuple_print(&no_ints);
    tuple_free(&no_ints);

    Tuple no_strings = tuple_remove_type(&tuple, TYPE_STRING);
    printf("After removing strings: ");
    tuple_print(&no_strings);
    tuple_free(&no_strings);

    Tuple no_floats = tuple_remove_type(&tuple, TYPE_FLOAT);
    printf("After removing floats: ");
    tuple_print(&no_floats);
    tuple_free(&no_floats);

    Tuple no_doubles = tuple_remove_type(&tuple, TYPE_DOUBLE);
    printf("After removing doubles: ");
    tuple_print(&no_doubles);
    tuple_free(&no_doubles);

    Tuple no_chars = tuple_remove_type(&tuple, TYPE_CHAR);
    printf("After removing chars: ");
    tuple_print(&no_chars);
    tuple_free(&no_chars);

    tuple_free(&tuple);
    return 0;
}