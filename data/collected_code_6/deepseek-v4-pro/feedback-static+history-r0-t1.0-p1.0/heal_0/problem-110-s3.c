#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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

static bool init_range_list(RangeList *list, int capacity) {
    list->ranges = (Range *)malloc((size_t)capacity * sizeof(Range));
    if (list->ranges == NULL) {
        return false;
    }
    list->count = 0;
    list->capacity = capacity;
    return true;
}

static bool add_range(RangeList *list, int start, int end) {
    if (list->count >= list->capacity) {
        int new_capacity = list->capacity * 2;
        Range *new_ranges = (Range *)realloc(list->ranges, (size_t)new_capacity * sizeof(Range));
        if (new_ranges == NULL) {
            return false;
        }
        list->ranges = new_ranges;
        list->capacity = new_capacity;
    }
    list->ranges[list->count].start = start;
    list->ranges[list->count].end = end;
    list->count++;
    return true;
}

static void free_range_list(RangeList *list) {
    free(list->ranges);
    list->ranges = NULL;
    list->count = 0;
    list->capacity = 0;
}

static Range *merge_ranges(const Range *ranges, int count, int *merged_count) {
    if (count == 0) {
        *merged_count = 0;
        return NULL;
    }

    Range *merged = (Range *)malloc((size_t)count * sizeof(Range));
    if (merged == NULL) {
        *merged_count = 0;
        return NULL;
    }

    int m_count = 0;
    int curr_start = ranges[0].start;
    int curr_end = ranges[0].end;

    for (int i = 1; i < count; i++) {
        if (ranges[i].start <= curr_end) {
            if (ranges[i].end > curr_end) {
                curr_end = ranges[i].end;
            }
        } else {
            merged[m_count].start = curr_start;
            merged[m_count].end = curr_end;
            m_count++;
            curr_start = ranges[i].start;
            curr_end = ranges[i].end;
        }
    }
    
    merged[m_count].start = curr_start;
    merged[m_count].end = curr_end;
    m_count++;

    Range *resized = (Range *)realloc(merged, (size_t)m_count * sizeof(Range));
    if (resized == NULL) {
        free(merged);
        *merged_count = 0;
        return NULL;
    }

    *merged_count = m_count;
    return resized;
}

static Range *find_missing_ranges(const Range *ranges, int count, int range_start, int range_end, int *missing_count) {
    if (ranges == NULL || count < 0 || range_start > range_end) {
        *missing_count = 0;
        return NULL;
    }

    if (count == 0) {
        Range *result = (Range *)malloc(sizeof(Range));
        if (result == NULL) {
            *missing_count = 0;
            return NULL;
        }
        result[0].start = range_start;
        result[0].end = range_end;
        *missing_count = 1;
        return result;
    }

    Range *sorted = (Range *)malloc((size_t)count * sizeof(Range));
    if (sorted == NULL) {
        *missing_count = 0;
        return NULL;
    }

    for (int i = 0; i < count; i++) {
        sorted[i] = ranges[i];
    }

    qsort(sorted, (size_t)count, sizeof(Range), compare_ranges);

    int merged_count = 0;
    Range *merged = merge_ranges(sorted, count, &merged_count);
    free(sorted);

    if (merged == NULL) {
        *missing_count = 0;
        return NULL;
    }

    RangeList missing_list;
    if (!init_range_list(&missing_list, 4)) {
        free(merged);
        *missing_count = 0;
        return NULL;
    }

    int current_pos = range_start;

    for (int i = 0; i < merged_count; i++) {
        if (merged[i].end < current_pos) {
            continue;
        }
        
        if (merged[i].start > current_pos) {
            int gap_end = merged[i].start - 1;
            if (gap_end > range_end) {
                gap_end = range_end;
            }
            if (!add_range(&missing_list, current_pos, gap_end)) {
                free_range_list(&missing_list);
                free(merged);
                *missing_count = 0;
                return NULL;
            }
            current_pos = merged[i].end + 1;
            if (current_pos > range_end) {
                break;
            }
        } else {
            if (merged[i].end >= range_end) {
                current_pos = range_end + 1;
                break;
            }
            current_pos = merged[i].end + 1;
        }
    }

    if (current_pos <= range_end) {
        if (!add_range(&missing_list, current_pos, range_end)) {
            free_range_list(&missing_list);
            free(merged);
            *missing_count = 0;
            return NULL;
        }
    }

    free(merged);

    if (missing_list.count == 0) {
        free_range_list(&missing_list);
        *missing_count = 0;
        return NULL;
    }

    *missing_count = missing_list.count;
    return missing_list.ranges;
}

int main(void) {
    Range input[] = {
        {2, 5},
        {8, 10},
        {15, 20}
    };
    int input_count = sizeof(input) / sizeof(input[0]);
    int range_start = 1;
    int range_end = 25;
    int missing_count = 0;

    Range *missing = find_missing_ranges(input, input_count, range_start, range_end, &missing_count);

    if (missing != NULL) {
        for (int i = 0; i < missing_count; i++) {
            printf("[%d, %d] ", missing[i].start, missing[i].end);
        }
        printf("\n");
        free(missing);
    } else {
        printf("No missing ranges or error occurred\n");
    }

    Range input2[] = {
        {1, 5},
        {3, 8},
        {10, 12}
    };
    int input2_count = sizeof(input2) / sizeof(input2[0]);
    missing_count = 0;

    missing = find_missing_ranges(input2, input2_count, 1, 15, &missing_count);

    if (missing != NULL) {
        for (int i = 0; i < missing_count; i++) {
            printf("[%d, %d] ", missing[i].start, missing[i].end);
        }
        printf("\n");
        free(missing);
    } else {
        printf("No missing ranges or error occurred\n");
    }

    Range *empty_input = NULL;
    missing_count = 0;
    missing = find_missing_ranges(empty_input, 0, 5, 10, &missing_count);

    if (missing != NULL) {
        for (int i = 0; i < missing_count; i++) {
            printf("[%d, %d] ", missing[i].start, missing[i].end);
        }
        printf("\n");
        free(missing);
    } else {
        printf("No missing ranges or error occurred\n");
    }

    return 0;
}