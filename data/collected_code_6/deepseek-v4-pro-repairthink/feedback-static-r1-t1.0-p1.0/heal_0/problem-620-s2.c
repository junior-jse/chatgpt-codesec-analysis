#include <stdio.h>
#include <stdlib.h>

int cmpfunc(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

int *largestDivisibleSubset(int *nums, int numsSize, int *returnSize) {
    if (numsSize == 0) {
        *returnSize = 0;
        return NULL;
    }
    
    qsort(nums, numsSize, sizeof(int), cmpfunc);
    
    int *dp = (int *)malloc(numsSize * sizeof(int));
    int *prev = (int *)malloc(numsSize * sizeof(int));
    
    if (dp == NULL || prev == NULL) {
        free(dp);
        free(prev);
        *returnSize = 0;
        return NULL;
    }
    
    for (int i = 0; i < numsSize; i++) {
        dp[i] = 1;
        prev[i] = -1;
    }
    
    int maxIndex = 0;
    int maxSize = 1;
    
    for (int i = 1; i < numsSize; i++) {
        for (int j = 0; j < i; j++) {
            if (nums[i] % nums[j] == 0 && dp[j] + 1 > dp[i]) {
                dp[i] = dp[j] + 1;
                prev[i] = j;
            }
        }
        if (dp[i] > maxSize) {
            maxSize = dp[i];
            maxIndex = i;
        }
    }
    
    int *result = (int *)malloc(maxSize * sizeof(int));
    if (result == NULL) {
        free(dp);
        free(prev);
        *returnSize = 0;
        return NULL;
    }
    
    *returnSize = maxSize;
    int index = maxIndex;
    int resultIndex = maxSize - 1;
    
    while (index >= 0) {
        result[resultIndex] = nums[index];
        resultIndex--;
        index = prev[index];
    }
    
    free(dp);
    free(prev);
    
    return result;
}

int main() {
    int nums[] = {1, 2, 4, 8};
    int numsSize = sizeof(nums) / sizeof(nums[0]);
    int returnSize = 0;
    
    int *result = largestDivisibleSubset(nums, numsSize, &returnSize);
    
    printf("Largest Divisible Subset: ");
    for (int i = 0; i < returnSize; i++) {
        printf("%d ", result[i]);
    }
    printf("\n");
    
    free(result);
    
    int nums2[] = {1, 2, 3};
    int numsSize2 = sizeof(nums2) / sizeof(nums2[0]);
    int returnSize2 = 0;
    
    int *result2 = largestDivisibleSubset(nums2, numsSize2, &returnSize2);
    
    printf("Largest Divisible Subset: ");
    for (int i = 0; i < returnSize2; i++) {
        printf("%d ", result2[i]);
    }
    printf("\n");
    
    free(result2);
    
    return 0;
}