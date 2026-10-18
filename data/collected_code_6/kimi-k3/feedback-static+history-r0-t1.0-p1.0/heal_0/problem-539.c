#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
    double *data;
    size_t size;
} List;

List *create_list(size_t size) {
    List *list = malloc(sizeof(List));
    if (list == NULL) {
        return NULL;
    }
    list->data = malloc(size * sizeof(double));
    if (list->data == NULL) {
        free(list);
        return NULL;
    }
    list->size = size;
    return list;
}

void free_list(List *list) {
    if (list != NULL) {
        free(list->data);
        free(list);
    }
}

double power_func(double base, double exponent) {
    return pow(base, exponent);
}

List *map_powers(const double *bases, size_t size) {
    if (bases == NULL || size == 0) {
        return NULL;
    }
    
    List *result = create_list(size);
    if (result == NULL) {
        return NULL;
    }
    
    for (size_t i = 0; i < size; i++) {
        result->data[i] = power_func(bases[i], (double)i);
    }
    
    return result;
}

int main(void) {
    double bases[] = {2.0, 3.0, 4.0, 5.0, 2.5};
    size_t size = sizeof(bases) / sizeof(bases[0]);
    
    List *powers = map_powers(bases, size);
    if (powers == NULL) {
        fprintf(stderr, "Error: Failed to create list\n");
        return EXIT_FAILURE;
    }
    
    printf("Powers: ");
    for (size_t i = 0; i < powers->size; i++) {
        printf("%.2f ", powers->data[i]);
    }
    printf("\n");
    
    free_list(powers);
    return EXIT_SUCCESS;
}