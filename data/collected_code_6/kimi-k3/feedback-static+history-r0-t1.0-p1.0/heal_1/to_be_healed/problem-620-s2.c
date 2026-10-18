#include <stdio.h>
#include <stdlib.h>

int compare(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

/* Possible weaknesses found:
 *  Parameter 'nums' can be declared as pointer to const [constParameterPointer]
 */
int largestDivisibleSubset(int *nums, int numsSize, int *result) {
    if (numsSize == 0) {
        return 0;
    }

    int *sorted = (int *)malloc(numsSize * sizeof(int));
    if (sorted == NULL) {
        return -1;
    }

    for (int i = 0; i < numsSize; i++) {
        sorted[i] = nums[i];
    }

    qsort(sorted, numsSize, sizeof(int), compare);

    int *dp = (int *)malloc(numsSize * sizeof(int));
    int *parent = (int *)malloc(numsSize * sizeof(int));

    if (dp == NULL || parent == NULL) {
        free(sorted);
        free(dp);
        free(parent);
        return -1;
    }

    int maxIndex = 0;
    int maxSize = 1;

    for (int i = 0; i < numsSize; i++) {
        dp[i] = 1;
        parent[i] = -1;
    }

    for (int i = 1; i < numsSize; i++) {
        for (int j = 0; j < i; j++) {
            if (sorted[i] % sorted[j] == 0 && dp[j] + 1 > dp[i]) {
                dp[i] = dp[j] + 1;
                parent[i] = j;
            }
        }
        if (dp[i] > maxSize) {
            maxSize = dp[i];
            maxIndex = i;
        }
    }

    int idx = maxIndex;
    int count = maxSize - 1;

    while (idx != -1) {
        result[count] = sorted[idx];
        idx = parent[idx];
        count--;
    }

    free(sorted);
    free(dp);
    free(parent);

    return maxSize;
}

int main(void) {
    int nums[] = {1, 2, 4, 8, 3, 9, 27};
    int numsSize = sizeof(nums) / sizeof(nums[0]);
    int *result = (int *)malloc(numsSize * sizeof(int));

    if (result == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    int size = largestDivisibleSubset(nums, numsSize, result);

    if (size < 0) {
        fprintf(stderr, "Error occurred\n");
        free(result);
        return 1;
    }

    printf("Largest divisible subset: ");
    for (int i = 0; i < size; i++) {
        printf("%d ", result[i]);
    }
    printf("\nSize: %d\n", size);

    free(result);
    return 0;
}