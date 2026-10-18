#include <stdio.h>
#include <stdlib.h>

int count_sequences(const int *values, int n, int k) {
    if (values == NULL || n <= 0 || k <= 0) {
        return 0;
    }

    int *current = (int *)calloc(k + 1, sizeof(int));
    int *next = (int *)calloc(k + 1, sizeof(int));

    if (current == NULL || next == NULL) {
        free(current);
        free(next);
        return 0;
    }

    current[0] = 1;

    for (int step = 1; step <= k; step++) {
        for (int j = 0; j <= k; j++) {
            next[j] = 0;
        }

        for (int min_sum = 0; min_sum <= step; min_sum++) {
            if (current[min_sum] == 0) {
                continue;
            }
            for (int i = 0; i < n; i++) {
                int new_min = min_sum + values[i];
                if (values[i] < 0 && -values[i] >= min_sum) {
                    new_min = 0;
                }
                if (new_min <= k) {
                    next[new_min] += current[min_sum];
                }
            }
        }

        int *swap = current;
        current = next;
        next = swap;
    }

    int total = current[0];
    free(current);
    free(next);

    return total;
}

int main(void) {
    int values[] = {1, -1};
    int n = sizeof(values) / sizeof(values[0]);
    int k = 4;

    int result = count_sequences(values, n, k);
    printf("%d\n", result);

    return 0;
}