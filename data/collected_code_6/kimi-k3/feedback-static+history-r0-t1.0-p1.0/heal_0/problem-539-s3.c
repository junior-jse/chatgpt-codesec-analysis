#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
    double *data;
    size_t size;
} DoubleList;

DoubleList* create_power_list(double base, const int *exponents, size_t count) {
    if (exponents == NULL || count == 0) {
        return NULL;
    }

    DoubleList *list = malloc(sizeof(DoubleList));
    if (list == NULL) {
        return NULL;
    }

    list->data = malloc(count * sizeof(double));
    if (list->data == NULL) {
        free(list);
        return NULL;
    }

    list->size = count;

    for (size_t i = 0; i < count; i++) {
        list->data[i] = pow(base, (double)exponents[i]);
    }

    return list;
}

void destroy_list(DoubleList *list) {
    if (list != NULL) {
        free(list->data);
        free(list);
    }
}

int main(void) {
    int exponents[] = {0, 1, 2, 3, 4, 5};
    size_t count = sizeof(exponents) / sizeof(exponents[0]);
    double base = 2.0;

    DoubleList *powers = create_power_list(base, exponents, count);
    
    if (powers == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    printf("Powers of %.1f: ", base);
    for (size_t i = 0; i < powers->size; i++) {
        printf("%.1f ", powers->data[i]);
    }
    printf("\n");

    destroy_list(powers);
    return EXIT_SUCCESS;
}