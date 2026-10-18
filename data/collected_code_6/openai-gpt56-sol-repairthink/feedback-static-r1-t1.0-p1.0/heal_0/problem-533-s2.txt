#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    TYPE_INT,
    TYPE_DOUBLE,
    TYPE_STRING
} DataType;

typedef struct {
    DataType type;
    union {
        int integer;
        double real;
        char *string;
    } value;
} Element;

typedef struct {
    Element *elements;
    size_t size;
} Tuple;

static char *duplicate_string(const char *source)
{
    size_t length;
    char *copy;

    if (source == NULL) {
        return NULL;
    }

    length = strlen(source);
    if (length == SIZE_MAX) {
        return NULL;
    }

    copy = malloc(length + 1);
    if (copy == NULL) {
        return NULL;
    }

    memcpy(copy, source, length + 1);
    return copy;
}

static void destroy_element(Element *element)
{
    if (element == NULL) {
        return;
    }

    if (element->type == TYPE_STRING) {
        free(element->value.string);
        element->value.string = NULL;
    }
}

static void destroy_tuple(Tuple *tuple)
{
    size_t i;

    if (tuple == NULL) {
        return;
    }

    for (i = 0; i < tuple->size; ++i) {
        destroy_element(&tuple->elements[i]);
    }

    free(tuple->elements);
    tuple->elements = NULL;
    tuple->size = 0;
}

static int remove_type(Tuple *tuple, DataType type)
{
    size_t read_index;
    size_t write_index = 0;
    Element *resized;

    if (tuple == NULL || (tuple->size > 0 && tuple->elements == NULL)) {
        return -1;
    }

    for (read_index = 0; read_index < tuple->size; ++read_index) {
        if (tuple->elements[read_index].type == type) {
            destroy_element(&tuple->elements[read_index]);
        } else {
            if (write_index != read_index) {
                tuple->elements[write_index] = tuple->elements[read_index];
            }
            ++write_index;
        }
    }

    if (write_index == 0) {
        free(tuple->elements);
        tuple->elements = NULL;
        tuple->size = 0;
        return 0;
    }

    tuple->size = write_index;
    resized = realloc(tuple->elements, write_index * sizeof(*tuple->elements));
    if (resized != NULL) {
        tuple->elements = resized;
    }

    return 0;
}

static void print_tuple(const Tuple *tuple)
{
    size_t i;

    if (tuple == NULL || (tuple->size > 0 && tuple->elements == NULL)) {
        return;
    }

    putchar('(');

    for (i = 0; i < tuple->size; ++i) {
        if (i > 0) {
            fputs(", ", stdout);
        }

        switch (tuple->elements[i].type) {
            case TYPE_INT:
                printf("%d", tuple->elements[i].value.integer);
                break;
            case TYPE_DOUBLE:
                printf("%g", tuple->elements[i].value.real);
                break;
            case TYPE_STRING:
                printf("\"%s\"", tuple->elements[i].value.string);
                break;
            default:
                fputs("<invalid>", stdout);
                break;
        }
    }

    puts(")");
}

int main(void)
{
    Tuple tuple = {0};

    tuple.size = 5;
    tuple.elements = calloc(tuple.size, sizeof(*tuple.elements));
    if (tuple.elements == NULL) {
        fputs("Failed to allocate tuple\n", stderr);
        return EXIT_FAILURE;
    }

    tuple.elements[0].type = TYPE_INT;
    tuple.elements[0].value.integer = 10;

    tuple.elements[1].type = TYPE_STRING;
    tuple.elements[1].value.string = duplicate_string("alpha");
    if (tuple.elements[1].value.string == NULL) {
        destroy_tuple(&tuple);
        fputs("Failed to allocate string\n", stderr);
        return EXIT_FAILURE;
    }

    tuple.elements[2].type = TYPE_DOUBLE;
    tuple.elements[2].value.real = 3.14;

    tuple.elements[3].type = TYPE_STRING;
    tuple.elements[3].value.string = duplicate_string("beta");
    if (tuple.elements[3].value.string == NULL) {
        destroy_tuple(&tuple);
        fputs("Failed to allocate string\n", stderr);
        return EXIT_FAILURE;
    }

    tuple.elements[4].type = TYPE_INT;
    tuple.elements[4].value.integer = 20;

    if (remove_type(&tuple, TYPE_STRING) != 0) {
        destroy_tuple(&tuple);
        fputs("Failed to remove elements\n", stderr);
        return EXIT_FAILURE;
    }

    print_tuple(&tuple);
    destroy_tuple(&tuple);

    return EXIT_SUCCESS;
}