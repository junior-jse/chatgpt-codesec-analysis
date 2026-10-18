#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int start;
    int end;
} Range;

int compare_ranges(const void *a, const void *b) {
    const Range *ra = (const Range *)a;
    const Range *rb = (const Range *)b;
    if (ra->start < rb->start) return -1;
    if (ra->start > rb->start) return 1;
    return 0;
}

Range* find_missing_ranges(Range *ranges, size_t count, int start, int end, size_t *result_count) {
    if (result_count == NULL || start > end) {
        return NULL;
    }
    
    *result_count = 0;
    
    if (count == 0 || ranges == NULL) {
        Range *result = (Range *)malloc(sizeof(Range));
        if (result == NULL) {
            return NULL;
        }
        result[0].start = start;
        result[0].end = end;
        *result_count = 1;
        return result;
    }
    
    qsort(ranges, count, sizeof(Range), compare_ranges);
    
    size_t capacity = count + 1;
    Range *result = (Range *)malloc(capacity * sizeof(Range));
    if (result == NULL) {
        return NULL;
    }
    
    size_t idx = 0;
    int current = start;
    
    for (size_t i = 0; i < count; i++) {
        if (ranges[i].end < current) {
            continue;
        }
        
        if (ranges[i].start > end) {
            break;
        }
        
        int range_start = ranges[i].start;
        int range_end = ranges[i].end;
        
        if (range_start > current) {
            result[idx].start = current;
            result[idx].end = range_start - 1;
            idx++;
        }
        
        if (range_end >= current) {
            current = range_end + 1;
        }
        
        if (current > end) {
            break;
        }
    }
    
    if (current <= end) {
        result[idx].start = current;
        result[idx].end = end;
        idx++;
    }
    
    *result_count = idx;
    
    if (idx == 0) {
        free(result);
        return NULL;
    }
    
    Range *final_result = (Range *)realloc(result, idx * sizeof(Range));
    if (final_result == NULL) {
        free(result);
        return NULL;
    }
    
    return final_result;
}

int main(void) {
    Range ranges[] = {{1, 3}, {5, 7}, {10, 12}};
    size_t count = sizeof(ranges) / sizeof(ranges[0]);
    int start = 0;
    int end = 15;
    size_t result_count = 0;
    
    Range *missing = find_missing_ranges(ranges, count, start, end, &result_count);
    
    if (missing != NULL) {
        printf("Missing ranges:\n");
        for (size_t i = 0; i < result_count; i++) {
            printf("[%d, %d]\n", missing[i].start, missing[i].end);
        }
        free(missing);
    } else {
        printf("No missing ranges or error occurred.\n");
    }
    
    return 0;
}