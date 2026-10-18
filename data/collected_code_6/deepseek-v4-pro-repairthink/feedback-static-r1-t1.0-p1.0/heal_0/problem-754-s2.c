#include <stdio.h>
#include <stdlib.h>

int* findCommonIndices(int *list1, int size1, int *list2, int size2, int *list3, int size3, int *resultSize) {
    if (list1 == NULL || list2 == NULL || list3 == NULL || resultSize == NULL) {
        return NULL;
    }
    
    if (size1 < 0 || size2 < 0 || size3 < 0) {
        return NULL;
    }
    
    int maxSize = size1;
    if (size2 > maxSize) maxSize = size2;
    if (size3 > maxSize) maxSize = size3;
    
    if (size1 == 0 || size2 == 0 || size3 == 0) {
        *resultSize = 0;
        return NULL;
    }
    
    int *commonIndices = (int*)malloc(sizeof(int) * maxSize);
    if (commonIndices == NULL) {
        return NULL;
    }
    
    int count = 0;
    int minSize = size1;
    if (size2 < minSize) minSize = size2;
    if (size3 < minSize) minSize = size3;
    
    for (int i = 0; i < minSize; i++) {
        if (list1[i] == list2[i] && list2[i] == list3[i]) {
            commonIndices[count++] = i;
        }
    }
    
    if (count == 0) {
        free(commonIndices);
        *resultSize = 0;
        return NULL;
    }
    
    int *result = (int*)realloc(commonIndices, sizeof(int) * count);
    if (result == NULL) {
        free(commonIndices);
        return NULL;
    }
    
    *resultSize = count;
    return result;
}

int main(void) {
    int list1[] = {1, 2, 3, 4, 5};
    int list2[] = {1, 3, 3, 6, 5};
    int list3[] = {1, 2, 3, 4, 5};
    
    int resultSize = 0;
    int *commonIndices = findCommonIndices(list1, 5, list2, 5, list3, 5, &resultSize);
    
    if (commonIndices != NULL && resultSize > 0) {
        printf("Common indices: ");
        for (int i = 0; i < resultSize; i++) {
            printf("%d ", commonIndices[i]);
        }
        printf("\n");
        free(commonIndices);
    } else {
        printf("No common indices found\n");
    }
    
    return 0;
}