#include <stdio.h>
#include <stdlib.h>

int* decodeRLE(const int* nums, int numsSize, int* returnSize) {
    if (nums == NULL || numsSize <= 0 || numsSize % 2 != 0 || returnSize == NULL) {
        if (returnSize != NULL) {
            *returnSize = 0;
        }
        return NULL;
    }
    
    int totalSize = 0;
    for (int i = 0; i < numsSize; i += 2) {
        if (nums[i] < 0) {
            *returnSize = 0;
            return NULL;
        }
        totalSize += nums[i];
    }
    
    if (totalSize == 0) {
        *returnSize = 0;
        return NULL;
    }
    
    int* result = (int*)malloc(totalSize * sizeof(int));
    if (result == NULL) {
        *returnSize = 0;
        return NULL;
    }
    
    int index = 0;
    for (int i = 0; i < numsSize; i += 2) {
        int freq = nums[i];
        int val = nums[i + 1];
        for (int j = 0; j < freq; j++) {
            result[index++] = val;
        }
    }
    
    *returnSize = totalSize;
    return result;
}

int main() {
    int nums[] = {1, 2, 3, 4};
    int numsSize = 4;
    int returnSize = 0;
    
    int* decoded = decodeRLE(nums, numsSize, &returnSize);
    
    if (decoded != NULL) {
        printf("Decoded array: ");
        for (int i = 0; i < returnSize; i++) {
            printf("%d ", decoded[i]);
        }
        printf("\n");
        free(decoded);
    } else {
        printf("Decoding failed or empty result\n");
    }
    
    return 0;
}