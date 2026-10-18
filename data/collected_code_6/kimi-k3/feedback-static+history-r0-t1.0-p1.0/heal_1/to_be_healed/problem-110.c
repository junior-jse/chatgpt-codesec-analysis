#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int start;
    int end;
} Range;

int compareRanges(const void *a, const void *b) {
    /* Possible weaknesses found:
     *  Variable 'rangeA' can be declared as pointer to const [constVariablePointer]
     */
    Range *rangeA = (Range *)a;
    /* Possible weaknesses found:
     *  Variable 'rangeB' can be declared as pointer to const [constVariablePointer]
     */
    Range *rangeB = (Range *)b;
    if (rangeA->start < rangeB->start) return -1;
    if (rangeA->start > rangeB->start) return 1;
    return 0;
}

Range* extractMissingRanges(Range *ranges, int count, int start, int end, int *missingCount) {
    /* Possible weaknesses found:
     *  Assuming that condition 'missingCount==NULL' is not redundant
     */
    if (ranges == NULL || missingCount == NULL || start > end) {
        /* Possible weaknesses found:
         *  Null pointer dereference
         *  Either the condition 'missingCount==NULL' is redundant or there is possible null pointer dereference: missingCount. [nullPointerRedundantCheck]
         */
        *missingCount = 0;
        return NULL;
    }

    Range *missing = (Range *)malloc(sizeof(Range) * (count + 1));
    if (missing == NULL) {
        *missingCount = 0;
        return NULL;
    }

    qsort(ranges, count, sizeof(Range), compareRanges);

    int current = start;
    int idx = 0;

    for (int i = 0; i < count; i++) {
        if (ranges[i].end < current) continue;
        if (ranges[i].start > end) break;

        if (ranges[i].start > current) {
            missing[idx].start = current;
            missing[idx].end = ranges[i].start - 1;
            idx++;
        }

        if (ranges[i].end + 1 > current) {
            current = ranges[i].end + 1;
        }
    }

    if (current <= end) {
        missing[idx].start = current;
        missing[idx].end = end;
        idx++;
    }

    *missingCount = idx;
    return missing;
}

int main() {
    Range ranges[] = {{1, 3}, {5, 7}, {10, 12}};
    int count = 3;
    int start = 0;
    int end = 15;
    int missingCount = 0;

    Range *missing = extractMissingRanges(ranges, count, start, end, &missingCount);

    if (missing != NULL) {
        for (int i = 0; i < missingCount; i++) {
            printf("[%d, %d]\n", missing[i].start, missing[i].end);
        }
        free(missing);
    }

    return 0;
}