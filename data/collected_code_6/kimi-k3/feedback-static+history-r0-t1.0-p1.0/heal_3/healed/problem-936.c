#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#define NAME_SIZE 50

typedef struct {
    int id;
    char name[NAME_SIZE];
    int value;
} Tuple;

static bool is_valid_name(const char *name)
{
    size_t i;
    
    if (name == NULL) {
        return false;
    }
    
    for (i = 0; i < NAME_SIZE; i++) {
        if (name[i] == '\0') {
            return true;
        }
    }
    
    return false;
}

static void safe_copy_name(char *dest, size_t dest_size, const char *src)
{
    size_t i;
    
    if (dest == NULL || dest_size == 0) {
        return;
    }
    
    if (src == NULL) {
        dest[0] = '\0';
        return;
    }
    
    for (i = 0; i < dest_size - 1 && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    dest[i] = '\0';
}

int rearrange_tuples(Tuple *tuples, size_t tuple_count, const size_t *order, size_t order_count)
{
    Tuple *temp;
    size_t i;
    
    if (tuples == NULL || order == NULL) {
        return -1;
    }
    
    if (tuple_count == 0 || order_count == 0) {
        return -1;
    }
    
    if (order_count > tuple_count) {
        return -1;
    }
    
    if (tuple_count > SIZE_MAX / sizeof(Tuple)) {
        return -1;
    }
    
    for (i = 0; i < tuple_count; i++) {
        if (!is_valid_name(tuples[i].name)) {
            return -1;
        }
    }
    
    temp = (Tuple *)malloc(tuple_count * sizeof(Tuple));
    if (temp == NULL) {
        return -1;
    }
    
    for (i = 0; i < order_count; i++) {
        if (order[i] >= tuple_count) {
            free(temp);
            temp = NULL;
            return -1;
        }
    }
    
    for (i = 0; i < tuple_count; i++) {
        temp[i] = tuples[i];
    }
    
    for (i = 0; i < order_count; i++) {
        tuples[i] = temp[order[i]];
    }
    
    free(temp);
    temp = NULL;
    
    return 0;
}

int init_tuple(Tuple *tuple, int id, const char *name, int value)
{
    if (tuple == NULL) {
        return -1;
    }
    
    tuple->id = id;
    safe_copy_name(tuple->name, NAME_SIZE, name);
    tuple->value = value;
    
    return 0;
}

void print_tuples(const Tuple *tuples, size_t count)
{
    size_t i;
    
    if (tuples == NULL) {
        return;
    }
    
    for (i = 0; i < count; i++) {
        printf("Tuple %zu: id=%d, name=%.49s, value=%d\n", 
               i, tuples[i].id, tuples[i].name, tuples[i].value);
    }
}

int main(void)
{
    Tuple tuples[5];
    size_t order[] = {2, 0, 4, 1, 3};
    size_t tuple_count = sizeof(tuples) / sizeof(tuples[0]);
    size_t order_count = sizeof(order) / sizeof(order[0]);
    int result;
    size_t i;
    
    const char *names[] = {"Alpha", "Beta", "Gamma", "Delta", "Epsilon"};
    int ids[] = {1, 2, 3, 4, 5};
    int values[] = {100, 200, 300, 400, 500};
    
    for (i = 0; i < tuple_count; i++) {
        if (init_tuple(&tuples[i], ids[i], names[i], values[i]) != 0) {
            fprintf(stderr, "Error: Failed to initialize tuple %zu\n", i);
            return EXIT_FAILURE;
        }
    }
    
    printf("Original tuples:\n");
    print_tuples(tuples, tuple_count);
    
    result = rearrange_tuples(tuples, tuple_count, order, order_count);
    
    if (result != 0) {
        fprintf(stderr, "Error: Failed to rearrange tuples\n");
        return EXIT_FAILURE;
    }
    
    printf("\nRearranged tuples:\n");
    print_tuples(tuples, tuple_count);
    
    return EXIT_SUCCESS;
}