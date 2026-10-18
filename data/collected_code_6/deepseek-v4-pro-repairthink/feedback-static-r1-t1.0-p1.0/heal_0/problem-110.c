#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int start;
    int end;
} Range;

typedef struct {
    Range *ranges;
    int count;
    int capacity;
} RangeList;

static int compare_ranges(const void *a, const void *b) {
    const Range *ra = (const Range *)a;
    const Range *rb = (const Range *)b;
    if (ra->start != rb->start) {
        return ra->start - rb->start;
    }
    return ra->end - rb->end;
}

static int ranges_overlap(const Range *a, const Range *b) {
    return a->start <= b->end && b->start <= a->end;
}

static Range merge_ranges(Range a, Range b) {
    Range result;
    result.start = a.start < b.start ? a.start : b.start;
    result.end = a.end > b.end ? a.end : b.end;
    return result;
}

static RangeList normalize_ranges(const Range *ranges, int count) {
    RangeList result;
    int i, j;
    
    result.capacity = count > 0 ? count : 1;
    result.ranges = (Range *)malloc(result.capacity * sizeof(Range));
    result.count = 0;
    
    if (count <= 0) {
        return result;
    }
    
    for (i = 0; i < count; i++) {
        result.ranges[i] = ranges[i];
    }
    result.count = count;
    
    qsort(result.ranges, result.count, sizeof(Range), compare_ranges);
    
    j = 0;
    for (i = 1; i < result.count; i++) {
        if (ranges_overlap(&result.ranges[j], &result.ranges[i]) ||
            result.ranges[j].end + 1 >= result.ranges[i].start) {
            result.ranges[j] = merge_ranges(result.ranges[j], result.ranges[i]);
        } else {
            j++;
            result.ranges[j] = result.ranges[i];
        }
    }
    result.count = j + 1;
    
    return result;
}

RangeList *find_missing_ranges(const Range *ranges, int count, int start_range, int end_range, int *missing_count) {
    RangeList normalized;
    RangeList *missing;
    int i;
    int current;
    
    if (missing_count == NULL) {
        return NULL;
    }
    
    if (start_range > end_range) {
        *missing_count = 0;
        return NULL;
    }
    
    normalized = normalize_ranges(ranges, count);
    
    missing = (RangeList *)malloc(sizeof(RangeList));
    if (missing == NULL) {
        free(normalized.ranges);
        *missing_count = 0;
        return NULL;
    }
    
    missing->capacity = normalized.count + 1;
    missing->ranges = (Range *)malloc(missing->capacity * sizeof(Range));
    if (missing->ranges == NULL) {
        free(normalized.ranges);
        free(missing);
        *missing_count = 0;
        return NULL;
    }
    missing->count = 0;
    
    current = start_range;
    
    for (i = 0; i < normalized.count; i++) {
        if (normalized.ranges[i].end < current) {
            continue;
        }
        
        if (normalized.ranges[i].start > end_range) {
            break;
        }
        
        if (normalized.ranges[i].start > current) {
            int missing_end = normalized.ranges[i].start - 1;
            if (missing_end > end_range) {
                missing_end = end_range;
            }
            if (missing->count >= missing->capacity) {
                missing->capacity *= 2;
                Range *temp = (Range *)realloc(missing->ranges, missing->capacity * sizeof(Range));
                if (temp == NULL) {
                    free(normalized.ranges);
                    free(missing->ranges);
                    free(missing);
                    *missing_count = 0;
                    return NULL;
                }
                missing->ranges = temp;
            }
            missing->ranges[missing->count].start = current;
            missing->ranges[missing->count].end = missing_end;
            missing->count++;
        }
        
        if (normalized.ranges[i].end >= end_range) {
            current = end_range + 1;
            break;
        }
        
        current = normalized.ranges[i].end + 1;
    }
    
    if (current <= end_range) {
        if (missing->count >= missing->capacity) {
            missing->capacity *= 2;
            Range *temp = (Range *)realloc(missing->ranges, missing->capacity * sizeof(Range));
            if (temp == NULL) {
                free(normalized.ranges);
                free(missing->ranges);
                free(missing);
                *missing_count = 0;
                return NULL;
            }
            missing->ranges = temp;
        }
        missing->ranges[missing->count].start = current;
        missing->ranges[missing->count].end = end_range;
        missing->count++;
    }
    
    free(normalized.ranges);
    *missing_count = missing->count;
    
    return missing;
}

void free_range_list(RangeList *list) {
    if (list == NULL) {
        return;
    }
    free(list->ranges);
    free(list);
}

int main(void) {
    Range input_ranges[] = {
        {2, 4},
        {8, 10},
        {15, 20}
    };
    int input_count = sizeof(input_ranges) / sizeof(input_ranges[0]);
    int start_range = 1;
    int end_range = 25;
    int missing_count = 0;
    RangeList *missing = find_missing_ranges(input_ranges, input_count, start_range, end_range, &missing_count);
    int i;
    
    printf("Missing ranges between %d and %d:\n", start_range, end_range);
    if (missing == NULL) {
        printf("None\n");
    } else {
        for (i = 0; i < missing_count; i++) {
            printf("[%d, %d]\n", missing->ranges[i].start, missing->ranges[i].end);
        }
        free_range_list(missing);
    }
    
    return 0;
}