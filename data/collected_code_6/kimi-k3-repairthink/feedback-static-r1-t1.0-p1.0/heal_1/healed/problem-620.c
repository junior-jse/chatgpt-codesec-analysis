#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

static int compare_ints(const void *a, const void *b)
{
    int x = *(const int *)a;
    int y = *(const int *)b;
    if (x < y) return -1;
    if (x > y) return 1;
    return 0;
}

int *largest_divisible_subset(const int *nums, int nums_size, int *result_size)
{
    if (nums == NULL || nums_size <= 0 || result_size == NULL) {
        if (result_size != NULL) {
            *result_size = 0;
        }
        return NULL;
    }

    size_t count = (size_t)nums_size;

    if (count > SIZE_MAX / sizeof(int)) {
        *result_size = 0;
        return NULL;
    }

    size_t buffer_size = count * sizeof(int);

    int *sorted = malloc(buffer_size);
    if (sorted == NULL) {
        *result_size = 0;
        return NULL;
    }

    for (size_t i = 0; i < count; i++) {
        sorted[i] = nums[i];
    }

    qsort(sorted, count, sizeof(int), compare_ints);

    int *dp = malloc(buffer_size);
    int *parent = malloc(buffer_size);
    if (dp == NULL || parent == NULL) {
        free(sorted);
        free(dp);
        free(parent);
        *result_size = 0;
        return NULL;
    }

    int max_idx = 0;
    int max_len = 1;

    for (size_t i = 0; i < count; i++) {
        dp[i] = 1;
        parent[i] = -1;
        for (size_t j = 0; j < i; j++) {
            if (sorted[j] != 0 && sorted[i] % sorted[j] == 0 && dp[j] + 1 > dp[i]) {
                dp[i] = dp[j] + 1;
                parent[i] = (int)j;
            }
        }
        if (dp[i] > max_len) {
            max_len = dp[i];
            max_idx = (int)i;
        }
    }

    int *result = malloc((size_t)max_len * sizeof(int));
    if (result == NULL) {
        free(sorted);
        free(dp);
        free(parent);
        *result_size = 0;
        return NULL;
    }

    int idx = max_idx;
    for (int i = max_len - 1; i >= 0; i--) {
        result[i] = sorted[idx];
        idx = parent[idx];
    }

    free(sorted);
    free(dp);
    free(parent);

    *result_size = max_len;
    return result;
}

int main(void)
{
    int nums[] = {1, 2, 4, 8, 16, 3, 9, 27};
    int nums_size = (int)(sizeof(nums) / sizeof(nums[0]));
    int result_size = 0;

    int *result = largest_divisible_subset(nums, nums_size, &result_size);

    if (result != NULL) {
        printf("Largest divisible subset (size %d): ", result_size);
        for (int i = 0; i < result_size; i++) {
            printf("%d ", result[i]);
        }
        printf("\n");
        free(result);
    } else {
        printf("No subset found or allocation failed.\n");
    }

    return 0;
}