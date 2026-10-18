#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int maxSumOfLists(int **lists, int listCount, int *listSizes) {
    int maxSum = INT_MIN;
    int i, j;
    
    for (i = 0; i < listCount; i++) {
        int currentSum = 0;
        for (j = 0; j < listSizes[i]; j++) {
            currentSum += lists[i][j];
        }
        if (currentSum > maxSum) {
            maxSum = currentSum;
        }
    }
    
    return maxSum;
}

int main() {
    int list1[] = {1, 2, 3};
    int list2[] = {4, 5};
    int list3[] = {6, 7, 8, 9};
    int list4[] = {-1, -2, -3};
    
    int *lists[] = {list1, list2, list3, list4};
    int listSizes[] = {3, 2, 4, 3};
    
    int result = maxSumOfLists(lists, 4, listSizes);
    
    printf("Maximum sum: %d\n", result);
    
    return 0;
}