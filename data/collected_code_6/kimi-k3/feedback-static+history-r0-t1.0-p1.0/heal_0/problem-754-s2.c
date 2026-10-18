#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int find_common_indices(const int *list1, size_t size1, const int *list2, size_t size2, const int *list3, size_t size3, size_t **indices, size_t *count) {
    if (list1 == NULL || list2 == NULL || list3 == NULL || indices == NULL || count == NULL) {
        return -1;
    }
    
    if (size1 == 0 || size2 == 0 || size3 == 0) {
        *indices = NULL;
        *count = 0;
        return 0;
    }
    
    size_t min_size = size1;
    if (size2 < min_size) min_size = size2;
    if (size3 < min_size) min_size = size3;
    
    size_t *temp = malloc(min_size * sizeof(size_t));
    if (temp == NULL) {
        return -1;
    }
    
    size_t found = 0;
    for (size_t i = 0; i < min_size; i++) {
        if (list1[i] == list2[i] && list2[i] == list3[i]) {
            temp[found] = i;
            found++;
        }
    }
    
    if (found == 0) {
        free(temp);
        *indices = NULL;
        *count = 0;
        return 0;
    }
    
    size_t *result = realloc(temp, found * sizeof(size_t));
    if (result == NULL) {
        free(temp);
        return -1;
    }
    
    *indices = result;
    *count = found;
    return 0;
}

int main(void) {
    int list1[] = {5, 2, 8, 1, 9};
    int list2[] = {3, 2, 8, 4, 9};
    int list3[] = {7, 2, 8, 6, 9};
    
    size_t size1 = sizeof(list1) / sizeof(list1[0]);
    size_t size2 = sizeof(list2) / sizeof(list2[0]);
    size_t size3 = sizeof(list3) / sizeof(list3[0]);
    
    size_t *indices = NULL;
    size_t count = 0;
    
    if (find_common_indices(list1, size1, list2, size2, list3, size3, &indices, &count) != 0) {
        fprintf(stderr, "Error: Memory allocation failed or invalid input\n");
        return EXIT_FAILURE;
    }
    
    if (count > 0) {
        printf("Common indices: ");
        for (size_t i = 0; i < count; i++) {
            printf("%zu", indices[i]);
            if (i < count - 1) {
                printf(", ");
            }
        }
        printf("\n");
        free(indices);
    } else {
        printf("No common elements found at the same indices.\n");
    }
    
    return EXIT_SUCCESS;
}