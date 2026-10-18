#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int start;
    int end;
} Range;

/* Possible weaknesses found:
 *  Parameter 'nums' can be declared as pointer to const [constParameterPointer]
 */
Range* find_missing_ranges(int* nums, int nums_size, int start, int end, int* result_size) {
    Range* missing = (Range*)malloc(((size_t)(end - start + 1) / 2 + 2) * sizeof(Range));
    if (missing == NULL) {
        *result_size = 0;
        return NULL;
    }

    *result_size = 0;
    int prev = start - 1;

    for (int i = 0; i <= nums_size; i++) {
        int curr;
        if (i < nums_size) {
            curr = nums[i];
        } else {
            curr = end + 1;
        }

        if (prev + 1 <= curr - 1) {
            missing[*result_size].start = prev + 1;
            missing[*result_size].end = curr - 1;
            (*result_size)++;
        }

        prev = curr;
    }

    return missing;
}

int main() {
    int nums[] = {3, 5, 10};
    int nums_size = sizeof(nums) / sizeof(nums[0]);
    int start = 0;
    int end = 15;
    int result_size = 0;

    Range* missing = find_missing_ranges(nums, nums_size, start, end, &result_size);

    if (missing != NULL) {
        for (int i = 0; i < result_size; i++) {
            printf("[%d, %d]\n", missing[i].start, missing[i].end);
        }
        free(missing);
    }

    return 0;
}