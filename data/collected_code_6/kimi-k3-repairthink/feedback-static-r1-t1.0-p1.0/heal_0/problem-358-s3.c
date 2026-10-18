#include <stdio.h>
#include <stdlib.h>

int* modulo_lists(const int *list1, const int *list2, size_t size) {
    if (list1 == NULL || list2 == NULL || size == 0) {
        return NULL;
    }
    
    int *result = (int*)malloc(size * sizeof(int));
    if (result == NULL) {
        return NULL;
    }
    
    for (size_t i = 0; i < size; i++) {
        if (list2[i] == 0) {
            free(result);
            return NULL;
        }
        result[i] = list1[i] % list2[i];
    }
    
    return result;
}

int main(void) {
    int list1[] = {10, 20, 30, 40, 50};
    int list2[] = {3, 4, 5, 6, 7};
    size_t size = sizeof(list1) / sizeof(list1[0]);
    
    int *result = modulo_lists(list1, list2, size);
    if (result == NULL) {
        fprintf(stderr, "Error: modulo operation failed\n");
        return EXIT_FAILURE;
    }
    
    for (size_t i = 0; i < size; i++) {
        printf("%d ", result[i]);
    }
    printf("\n");
    
    free(result);
    return EXIT_SUCCESS;
}