#include <stdio.h>
#include <stdlib.h>

int compare_ints(const void *a, const void *b) {
    int arg1 = *(const int *)a;
    int arg2 = *(const int *)b;
    return (arg1 > arg2) - (arg1 < arg2);
}

int *largest_divisible_subset(int *nums, int nums_size, int *return_size) {
    if (nums_size == 0) {
        *return_size = 0;
        return NULL;
    }

    qsort(nums, nums_size, sizeof(int), compare_ints);

    int *dp = (int *)malloc(nums_size * sizeof(int));
    int *prev = (int *)malloc(nums_size * sizeof(int));
    if (dp == NULL || prev == NULL) {
        free(dp);
        free(prev);
        *return_size = 0;
        return NULL;
    }

    for (int i = 0; i < nums_size; i++) {
        dp[i] = 1;
        prev[i] = -1;
    }

    int max_idx = 0;
    int max_len = 1;

    for (int i = 1; i < nums_size; i++) {
        for (int j = 0; j < i; j++) {
            if (nums[i] % nums[j] == 0 && dp[j] + 1 > dp[i]) {
                dp[i] = dp[j] + 1;
                prev[i] = j;
            }
        }
        if (dp[i] > max_len) {
            max_len = dp[i];
            max_idx = i;
        }
    }

    int *result = (int *)malloc(max_len * sizeof(int));
    if (result == NULL) {
        free(dp);
        free(prev);
        *return_size = 0;
        return NULL;
    }

    int idx = max_idx;
    int pos = max_len - 1;
    while (idx != -1) {
        result[pos--] = nums[idx];
        idx = prev[idx];
    }

    free(dp);
    free(prev);
    *return_size = max_len;
    return result;
}

int main(void) {
    int nums[] = {1, 2, 4, 8};
    int nums_size = sizeof(nums) / sizeof(nums[0]);
    int return_size = 0;

    int *result = largest_divisible_subset(nums, nums_size, &return_size);

    if (result != NULL) {
        for (int i = 0; i < return_size; i++) {
            printf("%d", result[i]);
            if (i < return_size - 1) {
                printf(" ");
            }
        }
        printf("\n");
        free(result);
    }

    return 0;
}