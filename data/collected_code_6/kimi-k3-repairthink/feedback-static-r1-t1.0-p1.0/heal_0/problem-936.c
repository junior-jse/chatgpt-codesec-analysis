#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int id;
    char name[50];
    int value;
} Tuple;

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
    
    temp = (Tuple *)malloc(tuple_count * sizeof(Tuple));
    if (temp == NULL) {
        return -1;
    }
    
    memcpy(temp, tuples, tuple_count * sizeof(Tuple));
    
    for (i = 0; i < order_count; i++) {
        if (order[i] >= tuple_count) {
            free(temp);
            temp = NULL;
            return -1;
        }
        tuples[i] = temp[order[i]];
    }
    
    free(temp);
    temp = NULL;
    
    return 0;
}

void print_tuples(const Tuple *tuples, size_t count)
{
    size_t i;
    
    if (tuples == NULL) {
        return;
    }
    
    for (i = 0; i < count; i++) {
        printf("Tuple %zu: id=%d, name=%s, value=%d\n", 
               i, tuples[i].id, tuples[i].name, tuples[i].value);
    }
}

int main(void)
{
    Tuple tuples[] = {
        {1, "Alpha", 100},
        {2, "Beta", 200},
        {3, "Gamma", 300},
        {4, "Delta", 400},
        {5, "Epsilon", 500}
    };
    
    size_t order[] = {2, 0, 4, 1, 3};
    size_t tuple_count = sizeof(tuples) / sizeof(tuples[0]);
    size_t order_count = sizeof(order) / sizeof(order[0]);
    int result;
    
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