#include <stdio.h>
#include <stdlib.h>

struct Range {
    int start;
    int end;
};

/* Possible weaknesses found:
 *  Parameter 'nums' can be declared as pointer to const [constParameterPointer]
 */
struct Range* find_missing_ranges(int* nums, int nums_size, int start, int end, int* result_size) {
    if (start > end) {
        *result_size = 0;
        return NULL;
    }

    int capacity = 32;
    struct Range* result = (struct Range*)malloc(capacity * sizeof(struct Range));
    if (result == NULL) {
        *result_size = 0;
        return NULL;
    }

    *result_size = 0;
    int prev = start - 1;

    for (int i = 0; i <= nums_size; i++) {
        int curr;
        if (i < nums_size) {
            if (nums[i] < start || nums[i] > end) {
                prev = nums[i];
                continue;
            }
            if (i > 0 && nums[i] <= nums[i-1]) {
                continue;
            }
            curr = nums[i];
        } else {
            curr = end + 1;
        }

        if (curr - prev > 1) {
            if (*result_size >= capacity) {
                capacity *= 2;
                struct Range* temp = (struct Range*)realloc(result, capacity * sizeof(struct Range));
                if (temp == NULL) {
                    free(result);
                    *result_size = 0;
                    return NULL;
                }
                result = temp;
            }
            result[*result_size].start = prev + 1;
            result[*result_size].end = curr - 1;
            (*result_size)++;
        }

        prev = curr;
    }

    return result;
}

int main() {
    int nums[] = {0, 1, 3, 50, 75};
    int nums_size = sizeof(nums) / sizeof(nums[0]);
    int start = 0;
    int end = 99;
    int result_size = 0;

    struct Range* ranges = find_missing_ranges(nums, nums_size, start, end, &result_size);

    if (ranges != NULL) {
        for (int i = 0; i < result_size; i++) {
            if (ranges[i].start == ranges[i].end) {
                printf("%d\n", ranges[i].start);
            } else {
                printf("%d->%d\n", ranges[i].start, ranges[i].end);
            }
        }
        free(ranges);
    }

    return 0;
}