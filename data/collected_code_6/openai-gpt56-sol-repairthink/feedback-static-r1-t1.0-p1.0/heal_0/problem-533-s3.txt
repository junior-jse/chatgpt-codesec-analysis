#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    ELEMENT_INT,
    ELEMENT_DOUBLE,
    ELEMENT_STRING
} ElementType;

typedef struct {
    ElementType type;
    union {
        int64_t integer;
        double real;
        char *string;
    } value;
} Element;

typedef struct {
    Element *elements;
    size_t length;
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

    copy = malloc(length + 1U);
    if (copy == NULL) {
        return NULL;
    }

    memcpy(copy, source, length + 1U);
    return copy;
}

static void destroy_element(Element *element)
{
    if (element == NULL) {
        return;
    }

    if (element->type == ELEMENT_STRING) {
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

    for (i = 0U; i < tuple->length; ++i) {
        destroy_element(&tuple->elements[i]);
    }

    free(tuple->elements);
    tuple->elements = NULL;
    tuple->length = 0U;
}

static int remove_type(Tuple *tuple, ElementType type)
{
    size_t read_index;
    size_t write_index = 0U;

    if (tuple == NULL || (tuple->length != 0U && tuple->elements == NULL)) {
        return -1;
    }

    for (read_index = 0U; read_index < tuple->length; ++read_index) {
        if (tuple->elements[read_index].type == type) {
            destroy_element(&tuple->elements[read_index]);
        } else {
            if (write_index != read_index) {
                tuple->elements[write_index] = tuple->elements[read_index];
            }
            ++write_index;
        }
    }

    tuple->length = write_index;

    if (write_index == 0U) {
        free(tuple->elements);
        tuple->elements = NULL;
    }

    return 0;
}

static int print_tuple(const Tuple *tuple)
{
    size_t i;

    if (tuple == NULL || (tuple->length != 0U && tuple->elements == NULL)) {
        return -1;
    }

    putchar('(');

    for (i = 0U; i < tuple->length; ++i) {
        if (i != 0U) {
            fputs(", ", stdout);
        }

        switch (tuple->elements[i].type) {
            case ELEMENT_INT:
                printf("%lld", (long long)tuple->elements[i].value.integer);
                break;
            case ELEMENT_DOUBLE:
                printf("%g", tuple->elements[i].value.real);
                break;
            case ELEMENT_STRING:
                printf("\"%s\"", tuple->elements[i].value.string);
                break;
            default:
                return -1;
        }
    }

    puts(")");
    return ferror(stdout) ? -1 : 0;
}

int main(void)
{
    Tuple tuple = { NULL, 0U };
    int status = EXIT_FAILURE;

    tuple.elements = calloc(5U, sizeof(*tuple.elements));
    if (tuple.elements == NULL) {
        fputs("Allocation failed\n", stderr);
        return EXIT_FAILURE;
    }

    tuple.length = 5U;

    tuple.elements[0].type = ELEMENT_INT;
    tuple.elements[0].value.integer = 42;

    tuple.elements[1].type = ELEMENT_STRING;
    tuple.elements[1].value.string = duplicate_string("alpha");
    if (tuple.elements[1].value.string == NULL) {
        fputs("String allocation failed\n", stderr);
        goto cleanup;
    }

    tuple.elements[2].type = ELEMENT_DOUBLE;
    tuple.elements[2].value.real = 3.14;

    tuple.elements[3].type = ELEMENT_STRING;
    tuple.elements[3].value.string = duplicate_string("beta");
    if (tuple.elements[3].value.string == NULL) {
        fputs("String allocation failed\n", stderr);
        goto cleanup;
    }

    tuple.elements[4].type = ELEMENT_INT;
    tuple.elements[4].value.integer = 7;

    if (remove_type(&tuple, ELEMENT_STRING) != 0) {
        fputs("Unable to remove elements\n", stderr);
        goto cleanup;
    }

    if (print_tuple(&tuple) != 0) {
        fputs("Unable to print tuple\n", stderr);
        goto cleanup;
    }

    status = EXIT_SUCCESS;

cleanup:
    destroy_tuple(&tuple);
    return status;
}