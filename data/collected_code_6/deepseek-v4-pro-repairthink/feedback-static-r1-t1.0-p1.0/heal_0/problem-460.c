#include <stdio.h>
#include <stdlib.h>

int* getFirstElements(int** sublists, int sublistCount, int* sublistSizes) {
    if (sublists == NULL || sublistSizes == NULL || sublistCount <= 0) {
        return NULL;
    }
    
    int* result = (int*)malloc(sublistCount * sizeof(int));
    if (result == NULL) {
        return NULL;
    }
    
    for (int i = 0; i < sublistCount; i++) {
        if (sublists[i] == NULL || sublistSizes[i] <= 0) {
            free(result);
            return NULL;
        }
        result[i] = sublists[i][0];
    }
    
    return result;
}

int main(void) {
    int list1[] = {10, 20, 30};
    int list2[] = {40, 50};
    int list3[] = {60};
    
    int* sublists[] = {list1, list2, list3};
    int sizes[] = {3, 2, 1};
    int count = 3;
    
    int* firstElements = getFirstElements(sublists, count, sizes);
    if (firstElements == NULL) {
        printf("Error retrieving first elements\n");
        return 1;
    }
    
    for (int i = 0; i < count; i++) {
        printf("%d ", firstElements[i]);
    }
    printf("\n");
    
    free(firstElements);
    return 0;
}