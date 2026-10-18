#include <stdio.h>
#include <stdlib.h>

struct Range {
    int start;
    int end;
};

struct Range* find_missing_ranges(int* nums, int nums_size, int lower, int upper, int* return_size) {
    struct Range* result = NULL;
    *return_size = 0;
    int capacity = 0;
    
    int prev = lower - 1;
    
    for (int i = 0; i <= nums_size; i++) {
        int curr;
        if (i == nums_size) {
            curr = upper + 1;
        } else {
            curr = nums[i];
        }
        
        if (curr < lower || (i < nums_size && curr > upper)) {
            return NULL;
        }
        
        if (prev != curr && prev + 1 <= curr - 1) {
            if (*return_size >= capacity) {
                capacity = capacity == 0 ? 1 : capacity * 2;
                struct Range* temp = realloc(result, capacity * sizeof(struct Range));
                if (!temp) {
                    free(result);
                    *return_size = 0;
                    return NULL;
                }
                result = temp;
            }
            result[*return_size].start = prev + 1;
            result[*return_size].end = curr - 1;
            (*return_size)++;
        }
        prev = curr;
    }
    
    return result;
}

int main() {
    int nums[] = {0, 1, 3, 50, 75};
    int lower = 0;
    int upper = 99;
    int return_size = 0;
    
    struct Range* result = find_missing_ranges(nums, sizeof(nums) / sizeof(nums[0]), lower, upper, &return_size);
    
    if (result) {
        for (int i = 0; i < return_size; i++) {
            printf("[%d, %d]\n", result[i].start, result[i].end);
        }
        free(result);
    } else {
        if (return_size == 0) {
            printf("No missing ranges or allocation failed.\n");
        }
    }
    
    return 0;
}