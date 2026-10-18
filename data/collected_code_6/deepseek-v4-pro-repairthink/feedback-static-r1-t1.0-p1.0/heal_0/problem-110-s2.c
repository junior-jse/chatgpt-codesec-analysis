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
    if (ra->start != rb->start)
        return ra->start - rb->start;
    return ra->end - rb->end;
}

static int init_range_list(RangeList *list, int initial_capacity) {
    if (initial_capacity <= 0)
        initial_capacity = 4;
    list->ranges = (Range *)malloc((size_t)initial_capacity * sizeof(Range));
    if (!list->ranges)
        return 0;
    list->count = 0;
    list->capacity = initial_capacity;
    return 1;
}

static int add_range(RangeList *list, int start, int end) {
    if (list->count >= list->capacity) {
        int new_capacity = list->capacity * 2;
        Range *new_ranges = (Range *)realloc(list->ranges, (size_t)new_capacity * sizeof(Range));
        if (!new_ranges)
            return 0;
        list->ranges = new_ranges;
        list->capacity = new_capacity;
    }
    list->ranges[list->count].start = start;
    list->ranges[list->count].end = end;
    list->count++;
    return 1;
}

static void free_range_list(RangeList *list) {
    free(list->ranges);
    list->ranges = NULL;
    list->count = 0;
    list->capacity = 0;
}

static RangeList find_missing_ranges(const Range *ranges, int count, int start_range, int end_range) {
    RangeList missing;
    if (!init_range_list(&missing, 4)) {
        missing.ranges = NULL;
        missing.count = 0;
        missing.capacity = 0;
        return missing;
    }

    if (count <= 0 || start_range > end_range) {
        if (start_range <= end_range) {
            if (!add_range(&missing, start_range, end_range)) {
                free_range_list(&missing);
                missing.ranges = NULL;
                missing.count = 0;
                missing.capacity = 0;
                return missing;
            }
        }
        return missing;
    }

    Range *sorted = (Range *)malloc((size_t)count * sizeof(Range));
    if (!sorted) {
        free_range_list(&missing);
        missing.ranges = NULL;
        missing.count = 0;
        missing.capacity = 0;
        return missing;
    }

    for (int i = 0; i < count; i++) {
        sorted[i] = ranges[i];
    }

    qsort(sorted, (size_t)count, sizeof(Range), compare_ranges);

    int current = start_range;

    for (int i = 0; i < count; i++) {
        int rstart = sorted[i].start;
        int rend = sorted[i].end;

        if (rend < current)
            continue;

        if (rstart > end_range)
            break;

        if (rstart > current) {
            int miss_start = current;
            int miss_end = rstart - 1;
            if (miss_end > end_range)
                miss_end = end_range;
            if (miss_start <= miss_end) {
                if (!add_range(&missing, miss_start, miss_end)) {
                    free(sorted);
                    free_range_list(&missing);
                    missing.ranges = NULL;
                    missing.count = 0;
                    missing.capacity = 0;
                    return missing;
                }
            }
        }

        if (rend >= end_range) {
            current = end_range + 1;
            break;
        }

        if (rend + 1 > current)
            current = rend + 1;
    }

    if (current <= end_range) {
        if (!add_range(&missing, current, end_range)) {
            free(sorted);
            free_range_list(&missing);
            missing.ranges = NULL;
            missing.count = 0;
            missing.capacity = 0;
            return missing;
        }
    }

    free(sorted);
    return missing;
}

int main(void) {
    Range input[] = {
        {2, 4},
        {5, 7},
        {10, 12},
        {15, 18},
        {20, 22}
    };
    int input_count = (int)(sizeof(input) / sizeof(input[0]));
    int start_range = 1;
    int end_range = 25;

    RangeList missing = find_missing_ranges(input, input_count, start_range, end_range);

    if (missing.ranges == NULL && missing.count == 0 && missing.capacity == 0) {
        printf("Memory allocation failed or no valid range list returned\n");
        return 1;
    }

    printf("Missing ranges between %d and %d:\n", start_range, end_range);
    for (int i = 0; i < missing.count; i++) {
        printf("[%d, %d]", missing.ranges[i].start, missing.ranges[i].end);
        if (i < missing.count - 1)
            printf(", ");
    }
    printf("\n");

    free_range_list(&missing);

    return 0;
}